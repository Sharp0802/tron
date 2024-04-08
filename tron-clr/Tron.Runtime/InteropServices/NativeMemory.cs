using System.Runtime.InteropServices;

namespace Tron.Runtime.InteropServices;

internal static class NativeMemory
{
    public static unsafe T* Alloc<T>() where T : unmanaged
    {
        return (T*)global::System.Runtime.InteropServices.NativeMemory.Alloc((UIntPtr)Marshal.SizeOf<T>());
    }

    public static unsafe void Free(void* p)
    {
        global::System.Runtime.InteropServices.NativeMemory.Free(p);
    }
}
