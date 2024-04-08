using GlmSharp;
using Tron.Runtime.InteropServices;

namespace Tron.Runtime.Specials;

public sealed class Camera(Actor actor) : Component(actor, CreateCamera(actor))
{
    private static IntPtr CreateCamera(Actor actor)
    {
        unsafe
        {
            var ptr = NativeMemory.Alloc<CodeGen.Camera>();
            CodeGen.Camera.__ctor__(ptr, actor.Pointer);
            return (IntPtr)ptr;
        }
    }

    public float Near
    {
        get
        {
            unsafe
            {
                return CodeGen.Camera.get__Near((CodeGen.Camera*)Pointer);
            }
        }
        set
        {
            unsafe
            {
                CodeGen.Camera.set__Near((CodeGen.Camera*)Pointer, value);
            }
        }
    }

    public float Far
    {
        get
        {
            unsafe
            {
                return CodeGen.Camera.get__Far((CodeGen.Camera*)Pointer);
            }
        }
        set
        {
            unsafe
            {
                CodeGen.Camera.set__Far((CodeGen.Camera*)Pointer, value);
            }
        }
    }

    public float FoV
    {
        get
        {
            unsafe
            {
                return CodeGen.Camera.get__FoV((CodeGen.Camera*)Pointer);
            }
        }
        set
        {
            unsafe
            {
                CodeGen.Camera.set__FoV((CodeGen.Camera*)Pointer, value);
            }
        }
    }

    public mat4 Projection
    {
        get
        {
            unsafe
            {
                return CodeGen.Camera.get__Projection((CodeGen.Camera*)Pointer);
            }
        }
    }

    public mat4 View
    {
        get
        {
            unsafe
            {
                return CodeGen.Camera.get__View((CodeGen.Camera*)Pointer);
            }
        }
    }

    public static Camera? Current { get; private set; }

    public void Use()
    {
        unsafe
        {
            CodeGen.Camera.Bind((CodeGen.Camera*)Pointer);
            Current = this;
        }
    }

    public override void OnCreated()
    {
    }

    public override void OnEnabled()
    {
    }

    public override void OnUpdated(float delta)
    {
    }

    public override void OnDisabled()
    {
    }

    public override void OnDestroyed()
    {
    }
}