using CsInterop;
using GlmSharp;
using Tron.Runtime.Abstraction;
using Tron.Runtime.InteropServices;

namespace Tron.Runtime.Primitives;

public class Program : IDisposable, IContextObject
{
    private                 int              _disposed = 0;
    private readonly unsafe CodeGen.Program* _pointer;

    public Program()
    {
        unsafe
        {
            _pointer = NativeMemory.Alloc<CodeGen.Program>();
            CodeGen.Program.__ctor__(_pointer);
        }
    }

    public void Link()
    {
        unsafe
        {
            CodeGen.Program.Link(_pointer);
        }
    }

    public void Use()
    {
        unsafe
        {
            CodeGen.Program.Use(_pointer);
        }
    }

    public int GetLocation(string name)
    {
        unsafe
        {
            var str = new NativeString(name);
            return CodeGen.Program.GetLocation(_pointer, &str);
        }
    }

    public Shader AttachShader(string name)
    {
        unsafe
        {
            var str = new NativeString(name);
            var ptr = CodeGen.Program.AttachShader(_pointer, &str);
            return new Shader(ptr);
        }
    }

    public void Validate()
    {
        unsafe
        {
            CodeGen.Program.Validate(_pointer);
        }
    }

    public void Set<T>(int loc, T value) where T : unmanaged
    {
        switch (value)
        {
            case int   v: CodeGen.ShaderInterop.Set_int(loc, v); break;
            case float v: CodeGen.ShaderInterop.Set_float(loc, v);    break;
            case vec2  v: CodeGen.ShaderInterop.Set_vec2(loc, v);     break;
            case vec3  v: CodeGen.ShaderInterop.Set_vec3(loc, v);     break;
            case vec4  v: CodeGen.ShaderInterop.Set_vec4(loc, v);     break;
            case mat2  v: CodeGen.ShaderInterop.Set_mat2(loc, v);     break;
            case mat3  v: CodeGen.ShaderInterop.Set_mat3(loc, v);     break;
            case mat4  v: CodeGen.ShaderInterop.Set_mat4(loc, v);     break;
            
            default:
                throw new NotSupportedException();
        }
    }

    public void PrintLog()
    {
        unsafe
        {
            CodeGen.Program.PrintLog(_pointer);
        }
    }

    public void PrintAll()
    {
        unsafe
        {
            CodeGen.Program.PrintAll(_pointer);
        }
    }

    public void Dispose()
    {
        if (Interlocked.CompareExchange(ref _disposed, 1, 0) != 0)
            return;

        unsafe
        {
            CodeGen.Program.__dtor__(_pointer);
        }
    }
}