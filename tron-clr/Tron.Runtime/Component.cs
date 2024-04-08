using Tron.Runtime.CodeGen;
using Tron.Runtime.InteropServices;

namespace Tron.Runtime;

/// <summary>
/// A module of <see cref="Actor"/> that has same event cycle with owner <see cref="Actor"/>.
/// </summary>
public abstract class Component : CObject
{
    internal unsafe CodeGen.Component* Pointer { get; }
    
    /// <summary>
    /// Gets the owner <see cref="Actor"/>.
    /// </summary>
    public Actor Actor { get; }
    
    private protected Component(Actor actor, IntPtr ptr)
    {
        unsafe
        {
            Pointer = (CodeGen.Component*)ptr;
            Actor   = actor;
        }
    }
    
    /// <summary>
    /// Initialize <see cref="Component"/>.
    /// </summary>
    /// <param name="actor">The owner of this <see cref="Component"/></param>
    protected Component(Actor actor)
    {
        Actor = actor;
        unsafe
        {
            Pointer = NativeMemory.Alloc<CodeGen.Component>();
            CodeGen.Component.__ctor__(
                Pointer,
                (ulong)GetType().TypeHandle.Value,
                actor.Pointer,
                new CObjectEvents(this)
            );
        }
    }
}