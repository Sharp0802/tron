#include <cstring>
#include <clang-c/Index.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <ranges>
#include <vector>

using dir_iter = std::filesystem::recursive_directory_iterator;

std::ostream& operator <<(std::ostream& stream, const CXString& str)
{
    stream << clang_getCString(str);
    clang_disposeString(str);
    return stream;
}

std::string ToString(const CXString str)
{
    std::string stl(clang_getCString(str));
    clang_disposeString(str);
    return stl;
}

bool Replace(std::string& str, const std::string& from, const std::string& to)
{
    const size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

std::string TypeToString(const CXType& type)
{
    const auto  handle = clang_getTypeSpelling(type);
    std::string str(clang_getCString(handle));

    // Handle string
    Replace(str, "std::string", "NativeString");

    // Handle reference/pointers
    auto refScopeCount = 0;
    for (auto& ch: str)
    {
        if (ch != '*' && ch != '&')
            continue;
        refScopeCount++;
        ch = ' ';
    }

    // Handle auto poitners
    for (size_t pos; (pos = str.find("ptr<")) != std::string::npos;)
        str.replace(pos, 4, "SharedPointer<");

    // Handle initializer list
    for (size_t pos; (pos = str.find("std::initializer_list<")) != std::string::npos;)
        str.replace(pos, 22, "InitializerList<");

    // Handle signed primitives
    while (Replace(str, "int8_t", "sbyte"))
    {
    }
    while (Replace(str, "int16_t", "short"))
    {
    }
    while (Replace(str, "int32_t", "int"))
    {
    }
    while (Replace(str, "int64_t", "long"))
    {
    }
    while (Replace(str, "ssize_t", "nint"))
    {
    }

    // Handle unsigned primitives
    while (Replace(str, "uint8_t", "byte"))
    {
    }
    while (Replace(str, "uint16_t", "ushort"))
    {
    }
    while (Replace(str, "uint32_t", "uint"))
    {
    }
    while (Replace(str, "uint64_t", "ulong"))
    {
    }
    while (Replace(str, "size_t", "nuint"))
    {
    }

    // Handle consts
    auto constScopeCount = 0;
    for (size_t pos; (pos = str.find("const")) != std::string::npos;)
    {
        str.replace(pos, 5, "");
        constScopeCount++;
    }

    // Handle glm namespaces
    for (size_t pos; (pos = str.find("glm::")) != std::string::npos;)
        str.replace(pos, 5, "");

    // Trimming
    for (size_t pos; (pos = str.find(' ')) != std::string::npos;)
        str.replace(pos, 1, "");

    // Combining
    std::stringstream ss;

    if (refScopeCount-- > 0)
        ss << (constScopeCount ? "in " : "ref ");
    ss << str;
    while (refScopeCount-- > 0)
        ss << '*';

    clang_disposeString(handle);
    return ss.str();
}

struct FunctionEntry
{
    std::string Class;
    std::string Name;
    std::string Mangling;

    bool                     Static = false;
    std::string              Return;
    std::vector<std::string> Parameters;

    [[nodiscard]]
    std::string ToCSDef() const
    {
        __thread static char buffer[65535];

        std::stringstream params;

        if (!Static)
            params << "ref " << Class << " self";
        if (!Parameters.empty())
            params << ", ";
        for (auto i = 0; i < Parameters.size(); ++i)
        {
            if (i != 0)
                params << ", ";
            params << Parameters[i];
        }

        std::string name(Name);
        if (Name == Class)
            name = "__ctor__";
        if (Name[0] == '~')
            name = "__dtor__";

        sprintf(buffer, R"(
namespace Tron.CodeGen
{
    internal partial struct CXX__%s
    {
        [DllImport("tron", ExactSpelling=true, EntryPoint="%s")]
        internal extern %s %s(%s);
    }
}
)", Class.data(), Mangling.data(), Return.data(), name.data(), params.str().data());

        return buffer;
    }
};

struct TypeEntry
{
    std::string  Name;
    std::int64_t Size = -1;

    [[nodiscard]]
    std::string ToCSDef() const
    {
        __thread static char buffer[65535];

        sprintf(buffer, R"(
namespace Tron.CodeGen
{
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    internal partial struct CXX__%s
    {
        private unsafe fixed byte _data[%lld];
    }
}
)", Name.data(), Size);

        return buffer;
    }
};

struct CGContext
{
    std::map<std::string, FunctionEntry> Functions;
    std::map<std::string, TypeEntry>     Types;
};

int main()
{
    CGContext cg;

    for (const auto& entry: dir_iter("inc/"))
    {
        if (entry.is_directory())
            continue;

        const char* args[] = {
            "-x", "c++",
            "-fdeclspec", "-fms-extensions",
            "-I./inc",

            "-I/usr/lib/gcc/x86_64-pc-linux-gnu/13.2.1/include/",
            "-I/usr/include/"
        };
        CXIndex index = clang_createIndex(0, 1);

        // Parse translation unit
        const auto unit = clang_parseTranslationUnit(
            index,
            entry.path().c_str(),
            args, std::size(args),
            nullptr, 0,
            CXTranslationUnit_None);
        if (!unit)
            std::cerr << "failed: " << entry.path() << std::endl;

        std::cout << "visit: " << entry.path() << std::endl;

        // Parse AST
        const auto cursor = clang_getTranslationUnitCursor(unit);
        clang_visitChildren(cursor, [](CXCursor c, CXCursor, CXClientData client)
        {
            auto& ctx = *static_cast<decltype(cg)*>(client);

            // Filter methods
            const auto kind = clang_getCursorKind(c);
            if (kind != CXCursor_CXXMethod &&
                kind != CXCursor_Constructor &&
                kind != CXCursor_Destructor &&
                kind != CXCursor_FunctionDecl &&
                kind != CXCursor_ConversionFunction)
                return CXChildVisit_Recurse;
            if (clang_CXXMethod_isDeleted(c))
                return CXChildVisit_Continue;

            // Must be in namespace
            const CXCursor parent = clang_getCursorSemanticParent(c);
            const CXCursor ns     = clang_getCursorKind(parent) == CXCursor_ClassDecl
                                    ? clang_getCursorSemanticParent(parent)
                                    : parent;
            if (clang_getCursorKind(ns) != CXCursor_Namespace)
                return CXChildVisit_Continue;

            // Namespace name must contains 'tron'
            const auto nsStr = clang_getCursorDisplayName(ns);
            // ReSharper disable once CppStringLiteralToCharPointerConversion
            if (memcmp(clang_getCString(nsStr), "tron", 4) != 0)
                return CXChildVisit_Continue;
            clang_disposeString(nsStr);


            FunctionEntry entry;

            // Import fields
            entry.Class    = ToString(clang_getCursorDisplayName(parent));
            entry.Static   = kind != CXCursor_CXXMethod && kind != CXCursor_Constructor && kind != CXCursor_Destructor;
            entry.Mangling = ToString(clang_Cursor_getMangling(c));
            entry.Name     = ToString(clang_getCursorSpelling(c));
            entry.Return   = TypeToString(clang_getResultType(clang_getCursorType(c)));

            ctx.Types[entry.Class] = { entry.Class, clang_Type_getSizeOf(clang_getCursorType(parent)) };

            const auto tFunc = clang_getCursorType(c);
            const auto nArgs = clang_Cursor_getNumArguments(c);

            entry.Parameters.reserve(nArgs);

            // Import parameters information
            for (auto i = 0; i < nArgs; ++i)
            {
                const auto arg = clang_Cursor_getArgument(c, i);

                std::stringstream ss;
                ss << TypeToString(clang_getArgType(tFunc, i)) << ' ' << clang_getCursorSpelling(arg);
                entry.Parameters.push_back(ss.str());
            }

            // Commit
            ctx.Functions[entry.Mangling] = entry;

            return CXChildVisit_Continue;
        }, &cg);
    }

    std::ofstream out("cg/cg.g.cs");

    out << R"(
using System;
using System.Runtime.InteropServices;
using CsInterop;
)";

    for (const auto& entry: std::views::values(cg.Functions))
        out << entry.ToCSDef();
    for (const auto& entry: std::views::values(cg.Types))
        out << entry.ToCSDef();
}
