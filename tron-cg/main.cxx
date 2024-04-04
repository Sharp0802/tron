#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <ranges>
#include <set>
#include <stack>
#include <string>
#include <shared_mutex>

#include <clang-c/Index.h>
#include <pstl/glue_execution_defs.h>

std::string ToString(const CXString cx)
{
    std::string str(clang_getCString(cx));
    clang_disposeString(cx);
    return str;
}

void Replace(std::string& str, const std::string& from, const std::string& to)
{
    size_t begin;
    while ((begin = str.find(from)) != std::string::npos)
        str.replace(begin, from.length(), to);
}

void Replace(std::string& str, const char from, const char to)
{
    for (char& ch: str)
        if (ch == from)
            ch = to;
}

std::string GetFullyQualifiedNamespace(CXCursor c)
{
    std::stack<std::string> stack;
    while (clang_getCursorKind(c) == CXCursor_Namespace)
    {
        stack.push(ToString(clang_getCursorSpelling(c)));
        c = clang_getCursorSemanticParent(c);
    }
    std::stringstream ss;
    while (!stack.empty())
    {
        ss << stack.top() << "::";
        stack.pop();
    }
    return ss.str();
}

void ConvertTypeFormat(std::string& type)
{
    Replace(type, '&', '*');
    Replace(type, "const", "");

    Replace(type, "uint8_t", "byte");
    Replace(type, "uint16_t", "ushort");
    Replace(type, "uint32_t", "uint");
    Replace(type, "uint64_t", "ulong");

    Replace(type, "int8_t", "sbyte");
    Replace(type, "int16_t", "short");
    Replace(type, "int32_t", "int");
    Replace(type, "int64_t", "long");

    Replace(type, "ssize_t", "nint");
    Replace(type, "size_t", "nuint");

    Replace(type, "GLuint", "uint");
    Replace(type, "GLint", "int");
    Replace(type, "GLfloat", "float");

    Replace(type, "string", "NativeString");
    Replace(type, "initializer_list", "InitializerList");
}

void ToFieldConvention(std::string& type)
{
    if (std::isupper(type[0]))
        type[0] = static_cast<char>(tolower(type[0]));
    if (!type.starts_with("m_"))
        type = "m_" + type;
}

std::string DeduceDecltype(const CXCursor& cls, const CXType& type)
{
    struct
    {
        std::string name;
        CXCursor cursor;
    } cb = {
                ToString(clang_getTypeSpelling(type)),
                CXCursor(CXCursor_InvalidCode)
            };
    if (!cb.name.contains("decltype("))
        return cb.name;

    Replace(cb.name, "decltype(", "");
    cb.name.resize(cb.name.size() - 1);
    ToFieldConvention(cb.name);

    // Special Fields
    if (cb.name == "m_localPosition")
        cb.name = "m_position";
    if (cb.name == "m_foV")
        cb.name = "m_fov";
    if (cb.name == "m_eBO")
        return "IndexBuffer*";
    if (cb.name == "m_vBO")
        return "VertexBuffer*";
    if (cb.name == "m_shouldClose")
        return "bool";
    if (cb.name == "m_matrix")
        return "glm::mat4";

    clang_Type_visitFields(
        clang_getCursorType(cls),
        [](const CXCursor c, void* data)
        {
            const auto& lcb = static_cast<decltype(cb)*>(data);
            if (ToString(clang_getCursorSpelling(c)) != lcb->name)
                return CXVisit_Continue;
            lcb->cursor = c;
            return CXVisit_Break;
        },
        &cb);

    if (cb.cursor.kind == CXCursor_InvalidCode)
        throw std::logic_error("'" + cb.name + "' not found in '" + ToString(clang_getCursorSpelling(cls)) + "'");

    return ToString(clang_getTypeSpelling(clang_getCursorType(cb.cursor)));
}

std::string TypeToString(const CXCursor& cls, const CXType& type)
{
    auto t = DeduceDecltype(cls, type);
    if (auto pos = t.rfind(':'); pos != std::string::npos)
        t.replace(0, pos + 1, "");
    ConvertTypeFormat(t);
    return t;
}

std::string GetScript(const CXType& type)
{
    thread_local char buffer[BUFSIZ];
    snprintf(
        buffer,
        std::size(buffer),
        R"(
namespace Tron.CodeGen
{
    internal unsafe partial struct %s
    {
        private fixed byte _value[%lld];
    }
}
)",
        ToString(clang_getCursorSpelling(clang_getTypeDeclaration(type))).data(),
        clang_Type_getSizeOf(type)
    );
    return { buffer };
}

std::string GetScript(const CXCursor& cursor)
{
    const auto cls  = clang_getCursorSemanticParent(cursor);
    const auto type = clang_getCursorType(cursor);

    std::stringstream ss;

    bool appendComma = false;
    if (clang_getCursorKind(cursor) != CXCursor_CXXMethod || !clang_CXXMethod_isStatic(cursor))
    {
        ss << ToString(clang_getCursorSpelling(cls)) << "* " << "@this";
        appendComma = true;
    }

    for (int i = 0; i < clang_Cursor_getNumArguments(cursor); ++i)
    {
        auto arg  = ToString(clang_getCursorSpelling(clang_Cursor_getArgument(cursor, i)));
        auto argT = TypeToString(cls, clang_getArgType(type, i));

        if (i != 0 || appendComma)
            ss << ", ";
        ss << argT << ' ' << arg;
    }

    auto ret = TypeToString(cls, clang_getCursorResultType(cursor));

    auto clsName = ToString(clang_getCursorSpelling(cls));
    auto fnName  = ToString(clang_getCursorSpelling(cursor));

    thread_local char buffer[BUFSIZ];
    if (clsName == fnName)
        fnName = "__ctor__";
    if ("~" + clsName == fnName)
        fnName = "__dtor__";

    snprintf(
        buffer,
        std::size(buffer),
        R"(
namespace Tron.CodeGen
{
    internal unsafe partial struct %s
    {
        [DllImport("tron", EntryPoint="%s", ExactSpelling=true)]
        internal static extern %s %s(%s);
    }
}
)",
        clsName.data(),
        ToString(clang_Cursor_getMangling(cursor)).data(),
        ret.data(),
        fnName.data(),
        ss.str().data()
    );

    return buffer;
}

std::string GetScript(const std::string& enumC, const std::vector<CXCursor>& enums)
{
    std::stringstream ss;
    ss << R"(
namespace Tron.CodeGen
{
    public enum )" << enumC << R"(
    {
)";
    for (const auto& decl: enums)
        ss << "        " << ToString(clang_getCursorSpelling(decl)) << " = " << clang_getEnumConstantDeclValue(decl) << ',' << std::endl;
    ss << R"(
    }
}
)";
    return ss.str();
}

struct Context
{
    std::set<CXIndex> Indices;
    std::set<CXTranslationUnit> Translations;
    std::map<std::string, std::vector<CXCursor>> Enums;
    std::map<std::string, CXCursor> Types;
    std::map<std::string, CXCursor> Functions;
};

struct LocalContext
{
    std::map<std::string, std::vector<CXCursor>> Enums;
    std::map<std::string, CXCursor> Types;
    std::map<std::string, CXCursor> Functions;
};

CXChildVisitResult HandleFunction(CXCursor c, LocalContext& llctx)
{
    const auto kind = clang_getCursorKind(c);
    if (kind != CXCursor_CXXMethod &&
        kind != CXCursor_Constructor &&
        kind != CXCursor_Destructor &&
        kind != CXCursor_ConversionFunction)
        return CXChildVisit_Recurse;
    if (clang_CXXMethod_isDeleted(c))
        return CXChildVisit_Continue;

    auto cls = clang_getCursorSemanticParent(c);
    if (clang_getCursorKind(cls) != CXCursor_ClassDecl)
        return CXChildVisit_Continue;

    auto ns = GetFullyQualifiedNamespace(clang_getCursorSemanticParent(cls));
    if (!ns.contains("tron"))
        return CXChildVisit_Continue;


    llctx.Types[ToString(clang_getCursorSpelling(cls))] = cls;
    llctx.Functions[ToString(clang_getCursorSpelling(c))] = c;

    return CXChildVisit_Continue;
}

CXChildVisitResult HandleEnum(CXCursor c, LocalContext& llctx)
{
    const auto kind = clang_getCursorKind(c);
    if (kind != CXCursor_EnumConstantDecl)
        return CXChildVisit_Recurse;

    auto enumC = clang_getCursorSemanticParent(c);
    if (clang_getCursorKind(enumC) != CXCursor_EnumDecl)
        return CXChildVisit_Continue;

    auto ns = GetFullyQualifiedNamespace(clang_getCursorSemanticParent(enumC));
    if (!ns.contains("tron"))
        return CXChildVisit_Continue;

    llctx.Enums[ToString(clang_getCursorSpelling(enumC))].push_back(c);

    return CXChildVisit_Continue;
}

int main(const int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr
                << "error: invalid argument count" << std::endl
                << "syntax: tron-cg <root-of-project> [<include-directories>...]";
        return -1;
    }

    std::string include(argv[1]);
    include += "/tron/inc";

    std::string root(include);
    root += "/oop";

    const char* s_argv[] = {
        "-D_DEBUG",
        "-I", include.data(),
        "-I/usr/lib/gcc/x86_64-pc-linux-gnu/13.2.1/include/",
        "-I/usr/include/",
        "-std=gnu++26", "-fcolor-diagnostics",
        "-fdeclspec", "-fms-extensions",
        "-x", "c++"
    };

    std::vector<std::filesystem::directory_entry> dirVec;
    for (const auto& entry: std::filesystem::recursive_directory_iterator(root))
        if (!entry.is_directory())
            dirVec.push_back(entry);

    std::shared_mutex mutex;
    Context ctx;

    volatile int c = 0;

#pragma omp parallel for
    for (const auto& entry: dirVec)
    {
        const auto index = clang_createIndex(false, false);
        const auto unit  = clang_parseTranslationUnit(
            index,
            entry.path().c_str(),
            s_argv, std::size(s_argv),
            nullptr, 0,
            CXTranslationUnit_None);

        LocalContext lctx;
        clang_visitChildren(clang_getTranslationUnitCursor(unit), [](CXCursor c, CXCursor, void* d)
        {
            auto& llctx = *static_cast<decltype(lctx)*>(d);
            if (auto r = HandleFunction(c, llctx); r != CXChildVisit_Recurse)
                return r;
            return HandleEnum(c, llctx);
        }, &lctx);

        {
            std::scoped_lock lock(mutex);
            ctx.Indices.emplace(index);
            ctx.Translations.emplace(unit);
            for (const auto& fn: lctx.Functions)
                ctx.Functions.emplace(fn);
            for (const auto& type: lctx.Types)
                ctx.Types.emplace(type);
            for (const auto& [key, value]: lctx.Enums)
                ctx.Enums[key] = value;

            std::cout << '[' << ++c << '/' << dirVec.size() << "] " << entry.path() << std::endl;
        }
    }

    std::vector<std::string> psb;
    psb.reserve(ctx.Types.size() + ctx.Functions.size() + ctx.Enums.size());

    for (const auto& cursor: ctx.Types | std::views::values)
        psb.push_back(GetScript(clang_getCursorType(cursor)));
    for (const auto& cursor: ctx.Functions | std::views::values)
        psb.push_back(GetScript(cursor));
    for (const auto& [key, value]: ctx.Enums)
        psb.push_back(GetScript(key, value));

    std::stringstream ss;
    ss << R"(
using System.Runtime.InteropServices;
using CsInterop;
using GlmSharp;

using GLenum = uint;
)";
    for (const auto& str: psb)
        ss << str << std::endl;

    std::ofstream of(std::string(argv[1]) + "/tron-clr/cg.g.cs");
    of << ss.str();

    for (const auto& entry: ctx.Translations)
        clang_disposeTranslationUnit(entry);
    for (const auto& entry: ctx.Indices)
        clang_disposeIndex(entry);
}
