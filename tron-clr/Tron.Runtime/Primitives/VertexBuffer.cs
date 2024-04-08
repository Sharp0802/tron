using System.Runtime.InteropServices;
using Tron.Runtime.Abstraction;
using Tron.Runtime.CodeGen;
using NativeMemory = Tron.Runtime.InteropServices.NativeMemory;

namespace Tron.Runtime.Primitives;

/// <summary>
/// An object that acts as VBO (vertex buffer object).
/// </summary>
public sealed class VertexBuffer : IContextObject, IDisposable
{
    private                 int                   _disposed;
    private readonly unsafe CodeGen.VertexBuffer* _pointer;

    /// <summary>
    /// Creates new vertex-buffer.
    /// </summary>
    /// <param name="target">The target of <see cref="VertexBuffer"/>.</param>
    /// <param name="usage">The usage of <see cref="VertexBuffer"/>.</param>
    public VertexBuffer(VertexBufferTarget target, VertexBufferUsage usage)
    {
        unsafe
        {
            _pointer = NativeMemory.Alloc<CodeGen.VertexBuffer>();
            CodeGen.VertexBuffer.__ctor__(_pointer, target, usage);
        }
    }

    /// <inheritdoc />
    public void Use()
    {
        unsafe
        {
            CodeGen.VertexBuffer.Bind(_pointer);
        }
    }

    /// <summary>
    /// Buffers data into <see cref="VertexBuffer"/>.
    /// </summary>
    /// <param name="data">The data to store</param>
    /// <typeparam name="T">The type of element</typeparam>
    public void Buffer<T>(Span<T> data) where T : unmanaged
    {
        unsafe
        {
            fixed (T* p = data)
                CodeGen.VertexBuffer.Buffer(
                    _pointer, 
                    p, 
                    (UIntPtr)(data.Length * Marshal.SizeOf<T>()));
        }
    }

    /// <summary>
    /// Buffers data into <see cref="VertexBuffer"/> partially.
    /// </summary>
    /// <param name="data">The data to store</param>
    /// <param name="offset">An offset of segment</param>
    /// <typeparam name="T">The type of element</typeparam>
    public void BufferSubData<T>(Span<T> data, int offset) where T : unmanaged
    {
        unsafe
        {
            fixed (T* p = data)
                CodeGen.VertexBuffer.BufferSubData(
                    _pointer, 
                    p, 
                    checked((UIntPtr)offset),
                    (UIntPtr)(data.Length * Marshal.SizeOf<T>()));
        }
    }

    /// <inheritdoc />
    public void Dispose()
    {
        if (Interlocked.CompareExchange(ref _disposed, 1, 0) != 0)
            return;
        
        unsafe
        {
            CodeGen.VertexBuffer.__dtor__(_pointer);
            NativeMemory.Free(_pointer);
        }
    }
}