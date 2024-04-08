using System.Runtime.InteropServices;

namespace Tron.Runtime.InteropServices;

public static class NativeMemory
{
    public static unsafe T* Alloc<T>() where T : unmanaged
    {
        return (T*)global::System.Runtime.InteropServices.NativeMemory.Alloc((UIntPtr)Marshal.SizeOf<T>());
    }
}