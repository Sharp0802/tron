using System.Runtime.InteropServices;
using CsInterop;
using Tron.Runtime.Abstraction;
using Tron.Runtime.CodeGen;
using NativeMemory = Tron.Runtime.InteropServices.NativeMemory;

namespace Tron.Runtime.Primitives;

public sealed class Texture2D : ITexture
{
    private readonly unsafe CodeGen.Texture2D* _pointer;
    
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

    public void Use()
    {
        unsafe
        {
            CodeGen.Texture2D.Bind(_pointer);
        }
    }

    public IntPtr Pointer
    {
        get
        {
            unsafe
            {
                return (IntPtr)_pointer;
            }
        }
    }

    public void Dispose()
    {
        unsafe
        {
            CodeGen.Texture2D.__dtor__(_pointer);
        }
    }
}