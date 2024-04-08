using Tron.Runtime.Abstraction;
using Tron.Runtime.CodeGen;
using Tron.Runtime.InteropServices;

namespace Tron.Runtime.Primitives;

public class Material : IContextObject
{
    private readonly unsafe CodeGen.Material* _pointer;
    private readonly        Program           _program;
    private                 ITexture?         _texture;

    public Material()
    {
        unsafe
        {
            _pointer = NativeMemory.Alloc<CodeGen.Material>();
            CodeGen.Material.__ctor__(_pointer);
        }
    }

    public void Use()
    {
        unsafe
        {
            CodeGen.Material.Bind(_pointer);
        }
    }

    public Program Program => _program;

    public ITexture? Texture
    {
        get => _texture;
        set
        {
            unsafe
            {
                CodeGen.Material.set__Texture(_pointer, (Texture*)value?.Pointer);
                _texture = value;
            }
        }
    }
}