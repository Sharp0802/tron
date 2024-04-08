using Tron.Runtime.Abstraction;
using Tron.Runtime.InteropServices;
using Texture = Tron.Runtime.Abstraction.Texture;

namespace Tron.Runtime.Primitives;

/// <summary>
/// A material object contains shader-<see cref="Program"/> and <see cref="Texture"/>.
/// </summary>
public sealed class Material : IContextObject
{
    private readonly unsafe CodeGen.Material* _pointer;
    private readonly        Program           _program;
    private                 Texture?         _texture;

    /// <summary>
    /// Creates new <see cref="Material"/>.
    /// </summary>
    public Material()
    {
        unsafe
        {
            _pointer = NativeMemory.Alloc<CodeGen.Material>();
            CodeGen.Material.__ctor__(_pointer);
            _program = new Program(CodeGen.Material.get__Program(_pointer));
        }
    }

    ~Material()
    {
        unsafe
        {
            NativeMemory.Free(_pointer);
        }
    }
    
    /// <inheritdoc />
    public void Use()
    {
        unsafe
        {
            CodeGen.Material.Bind(_pointer);
        }
    }

    /// <summary>
    /// Gets coupled <see cref="Program"/>.
    /// </summary>
    public Program Program => _program;

    /// <summary>
    /// Gets or sets <see cref="Texture"/> of <see cref="Material"/>.
    /// </summary>
    public Texture? Texture
    {
        get => _texture;
        set
        {
            unsafe
            {
                CodeGen.Material.set__Texture(_pointer, (CodeGen.Texture*)value?.Pointer);
                _texture = value;
            }
        }
    }
}