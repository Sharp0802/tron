using Tron.Runtime.CodeGen;
using Tron.Runtime.InteropServices;

namespace Tron.Runtime;

public abstract class Component : CObject
{
    internal unsafe CodeGen.Component* Pointer { get; }
    
    public Actor Actor { get; }

    protected internal Component(Actor actor, IntPtr ptr)
    {
        unsafe
        {
            Pointer = (CodeGen.Component*)ptr;
            Actor   = actor;
        }
    }
    
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