using CsInterop;
using GlmSharp;
using Tron.Runtime.Abstraction;

namespace Tron.Runtime.Primitives;

/// <summary>
/// A shader-program object.
/// </summary>
public sealed class Program : IDisposable, IContextObject
{
    private                 int              _disposed;
    private readonly unsafe CodeGen.Program* _pointer;
    
    internal unsafe Program(CodeGen.Program* ptr)
    {
        _pointer = ptr;
    }

    /// <summary>
    /// Links <see cref="Shader"/>s.
    /// </summary>
    public void Link()
    {
        unsafe
        {
            CodeGen.Program.Link(_pointer);
        }
    }
    
    /// <inheritdoc />
    public void Use()
    {
        unsafe
        {
            CodeGen.Program.Use(_pointer);
        }
    }

    /// <summary>
    /// Gets location of <see cref="Shader"/> field.
    /// </summary>
    /// <param name="name">The name of field to find</param>
    /// <returns>Location of specified field</returns>
    public int GetLocation(string name)
    {
        unsafe
        {
            var str = new NativeString(name);
            return CodeGen.Program.GetLocation(_pointer, &str);
        }
    }

    /// <summary>
    /// Attaches <see cref="Shader"/> to <see cref="Program"/> by its path.
    /// </summary>
    /// <param name="name">The path of <see cref="Shader"/></param>
    /// <returns>Attached <see cref="Shader"/></returns>
    public Shader AttachShader(string name)
    {
        unsafe
        {
            var str = new NativeString(name);
            var ptr = CodeGen.Program.AttachShader(_pointer, &str);
            return new Shader(ptr);
        }
    }

    /// <summary>
    /// Validates <see cref="Program"/>.
    /// </summary>
    public void Validate()
    {
        unsafe
        {
            CodeGen.Program.Validate(_pointer);
        }
    }

    /// <summary>
    /// Sets <see cref="Shader"/> field with value.
    /// </summary>
    /// <param name="loc">The location of field.</param>
    /// <param name="value">A value to use</param>
    /// <typeparam name="T">The type of field</typeparam>
    /// <exception cref="NotSupportedException">
    /// Type of <paramref name="value"/> is not supported by engine.
    /// </exception>
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

    /// <summary>
    /// Prints logs of <see cref="Program"/>.
    /// </summary>
    public void PrintLog()
    {
        unsafe
        {
            CodeGen.Program.PrintLog(_pointer);
        }
    }

    /// <summary>
    /// Prints information of <see cref="Program"/>.
    /// </summary>
    public void PrintAll()
    {
        unsafe
        {
            CodeGen.Program.PrintAll(_pointer);
        }
    }

    /// <summary>
    /// Disposes <see cref="Program"/>.
    /// </summary>
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