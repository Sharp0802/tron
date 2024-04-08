using System.Runtime.InteropServices;

namespace Tron.Runtime.CodeGen;

[Serializable]
[StructLayout(LayoutKind.Sequential)]
internal struct CObjectEvents
{
    public delegate void CObjectEventHandler(IntPtr _);

    public delegate void UpdateEventHandler(IntPtr _, float delta);

    public CObjectEvents(Tron.Runtime.CObject obj) : this(
        _ => obj.OnCreated(),
        _ => obj.OnEnabled(),
        (_, delta) => obj.OnUpdated(delta),
        _ => obj.OnDisabled(),
        _ => obj.OnDestroyed())
    {
    }

    public CObjectEvents(
        CObjectEventHandler created,
        CObjectEventHandler enabled,
        UpdateEventHandler  updated,
        CObjectEventHandler disabled,
        CObjectEventHandler destroyed)
    {
        unsafe
        {
            Created   = (delegate* unmanaged[Cdecl]<CObject*>)Marshal.GetFunctionPointerForDelegate(created);
            Enabled   = (delegate* unmanaged[Cdecl]<CObject*>)Marshal.GetFunctionPointerForDelegate(enabled);
            Updated   = (delegate* unmanaged[Cdecl]<CObject*, float>)Marshal.GetFunctionPointerForDelegate(updated);
            Disabled  = (delegate* unmanaged[Cdecl]<CObject*>)Marshal.GetFunctionPointerForDelegate(disabled);
            Destroyed = (delegate* unmanaged[Cdecl]<CObject*>)Marshal.GetFunctionPointerForDelegate(destroyed);
        }
    }

    internal unsafe delegate* unmanaged[Cdecl]<CObject*>        Created;
    internal unsafe delegate* unmanaged[Cdecl]<CObject*>        Enabled;
    internal unsafe delegate* unmanaged[Cdecl]<CObject*, float> Updated;
    internal unsafe delegate* unmanaged[Cdecl]<CObject*>        Disabled;
    internal unsafe delegate* unmanaged[Cdecl]<CObject*>        Destroyed;
}