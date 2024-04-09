using CsInterop;
using Tron.Runtime.CodeGen;
using Tron.Runtime.InteropServices;

namespace Tron.Runtime.Primitives;

/// <summary>
/// An object that acts as VBO (vertex buffer object) containing indices.
/// </summary>
public class IndexBuffer
{
    private                 int                  _disposed;
    private readonly unsafe CodeGen.IndexBuffer* _pointer;
    private readonly        bool                 _external;

    internal unsafe IndexBuffer(CodeGen.IndexBuffer* ptr)
    {
        _pointer  = ptr;
        _external = true;
    }
    
    /// <summary>
    /// Creates new index-buffer.
    /// </summary>
    /// <param name="usage">The usage of the buffer.</param>
    public IndexBuffer(VertexBufferUsage usage)
    {
        unsafe
        {
            _pointer = NativeMemory.Alloc<CodeGen.IndexBuffer>();
            CodeGen.IndexBuffer.__ctor__(_pointer, usage);
        }
    }

    ~IndexBuffer()
    {
        if (Interlocked.CompareExchange(ref _disposed, 1, 0) != 0)
            return;
        if (_external)
            return;
        
        unsafe
        {
            NativeMemory.Free(_pointer);
        }
    }

    /// <summary>
    /// Buffers indices.
    /// </summary>
    /// <param name="indices">The indices in <see cref="byte"/></param>
    public void Buffer(Span<byte> indices)
    {
        unsafe
        {
            CodeGen.IndexBuffer.Buffer(_pointer, new InitializerList<byte>(indices));
        }
    }

    /// <summary>
    /// Buffers indices.
    /// </summary>
    /// <param name="indices">The indices in <see cref="ushort"/></param>
    public void Buffer(Span<ushort> indices)
    {
        unsafe
        {
            CodeGen.IndexBuffer.Buffer(_pointer, new InitializerList<ushort>(indices));
        }
    }

    /// <summary>
    /// Buffers indices.
    /// </summary>
    /// <param name="indices">The indices in <see cref="uint"/></param>
    public void Buffer(Span<uint> indices)
    {
        unsafe
        {
            CodeGen.IndexBuffer.Buffer(_pointer, new InitializerList<uint>(indices));
        }
    }
}