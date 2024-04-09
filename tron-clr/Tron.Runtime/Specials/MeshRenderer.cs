using Tron.Runtime.Abstraction;
using Tron.Runtime.InteropServices;

namespace Tron.Runtime.Specials;

/// <summary>
/// An object that acts as mesh-renderer on engine.
/// </summary>
public class MeshRenderer : Component, IDrawable, IContextObject
{
    private readonly unsafe CodeGen.MeshRenderer* _pointer;
    
    /// <summary>
    /// Creates new mesh renderer.
    /// </summary>
    /// <param name="actor">The <see cref="Actor"/> owning this</param>
    public MeshRenderer(Actor actor) : base(actor)
    {
        unsafe
        {
            _pointer = NativeMemory.Alloc<CodeGen.MeshRenderer>();
            CodeGen.MeshRenderer.__ctor__(_pointer, actor.Pointer);
        }
    }
    
    /// <inheritdoc />
    public void Draw()
    {
        unsafe
        {
            CodeGen.MeshRenderer.Draw(_pointer);
        }
    }

    /// <inheritdoc />
    public void Use()
    {
        unsafe
        {
            CodeGen.MeshRenderer.Bind(_pointer);
        }
    }

    ~MeshRenderer()
    {
        unsafe
        {
            NativeMemory.Free(_pointer);
        }
    }

    /// <inheritdoc />
    public override void OnCreated()
    {
    }

    /// <inheritdoc />
    public override void OnEnabled()
    {
    }

    /// <inheritdoc />
    public override void OnUpdated(float delta)
    {
    }

    /// <inheritdoc />
    public override void OnDisabled()
    {
    }

    /// <inheritdoc />
    public override void OnDestroyed()
    {
    }
}