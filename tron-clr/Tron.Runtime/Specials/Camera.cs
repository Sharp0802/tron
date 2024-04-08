using GlmSharp;
using Tron.Runtime.InteropServices;

namespace Tron.Runtime.Specials;

/// <summary>
/// A built-in <see cref="Component"/> that acts as a Camera.
/// </summary>
/// <param name="actor">The owner of this <see cref="Component"/></param>
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

    /// <summary>
    /// Gets or sets near distance of frustum
    /// </summary>
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

    /// <summary>
    /// Gets or sets far distance of frustum
    /// </summary>
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

    /// <summary>
    /// Gets or sets FoV (field of view) of <see cref="Camera"/> 
    /// </summary>
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

    /// <summary>
    /// Gets projection matrix for <see cref="Camera"/> 
    /// </summary>
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

    /// <summary>
    /// Gets view matrix for <see cref="Camera"/> 
    /// </summary>
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

    /// <summary>
    /// Gets current bound <see cref="Camera"/>
    /// </summary>
    public static Camera? Current { get; private set; }

    /// <summary>
    /// Use this camera as primary
    /// </summary>
    public void Use()
    {
        unsafe
        {
            CodeGen.Camera.Bind((CodeGen.Camera*)Pointer);
            Current = this;
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