using CsInterop;
using Tron.Runtime.Abstraction;
using Tron.Runtime.InteropServices;

namespace Tron.Runtime.Primitives;

/// <summary>
/// An object that acts as VAO (vertex array object).
/// </summary>
public sealed class VertexArray : IContextObject, IDisposable
{
    private                 int                  _disposed;
    private readonly unsafe CodeGen.VertexArray* _pointer;
    
    /// <summary>
    /// Initializes <see cref="VertexArray"/> with associated <see cref="VertexAttributeInfo"/>s.
    /// </summary>
    /// <param name="attributes"></param>
    public VertexArray(params VertexAttributeInfo[] attributes)
    {
        unsafe
        {
            var il = new InitializerList<VertexAttributeInfo>(attributes);
            _pointer = NativeMemory.Alloc<CodeGen.VertexArray>();
            CodeGen.VertexArray.__ctor__(_pointer, il);
        }
    }

    /// <inheritdoc />
    public void Use()
    {
        unsafe
        {
            CodeGen.VertexArray.Bind(_pointer);
        }
    }

    /// <inheritdoc />
    public void Dispose()
    {
        if (Interlocked.CompareExchange(ref _disposed, 1, 0) != 0)
            return;

        unsafe
        {
            CodeGen.VertexArray.__dtor__(_pointer);
            NativeMemory.Free(_pointer);
        }
    }
}