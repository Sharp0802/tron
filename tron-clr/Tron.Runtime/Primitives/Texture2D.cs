using CsInterop;
using Tron.Runtime.CodeGen;
using NativeMemory = Tron.Runtime.InteropServices.NativeMemory;
using Texture = Tron.Runtime.Abstraction.Texture;

namespace Tron.Runtime.Primitives;

/// <summary>
/// A texture object that contains 2D texture data.
/// </summary>
public sealed class Texture2D : Texture
{
    private                 int                _disposed;
    private readonly unsafe CodeGen.Texture2D* _pointer;
    
    /// <summary>
    /// Creates new <see cref="CodeGen.Texture"/>.
    /// </summary>
    /// <param name="img">An image path of <see cref="CodeGen.Texture"/></param>
    /// <param name="wrap">An option specifying how to wrap texture</param>
    /// <param name="filter">An option specifying how to filter texture</param>
    /// <param name="format">A format of texture</param>
    /// <param name="type">The type of texture</param>
    public Texture2D(
        string        img,
        TextureWrap   wrap   = TextureWrap.DEFAULT,
        TextureFilter filter = TextureFilter.DEFAULT,
        TextureFormat format = TextureFormat.DEFAULT,
        TextureType   type   = TextureType.DEFAULT)
    {
        unsafe
        {
            var str = new NativeString(img);
            _pointer = NativeMemory.Alloc<CodeGen.Texture2D>();
            CodeGen.Texture2D.__ctor__(_pointer, &str, wrap, filter, format, type);
        }
    }

    /// <inheritdoc />
    public override void Use()
    {
        unsafe
        {
            CodeGen.Texture2D.Bind(_pointer);
        }
    }

    internal override IntPtr Pointer
    {
        get
        {
            unsafe
            {
                return (IntPtr)_pointer;
            }
        }
    }

    /// <inheritdoc />
    public override void Dispose()
    {
        if (Interlocked.CompareExchange(ref _disposed, 1, 0) != 0)
            return;
        
        unsafe
        {
            CodeGen.Texture2D.__dtor__(_pointer);
            NativeMemory.Free(_pointer);
        }
    }
}