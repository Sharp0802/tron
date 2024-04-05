using System.Runtime.InteropServices;

namespace Tron.Runtime;

[Serializable]
[StructLayout(LayoutKind.Sequential)]
public struct VertexAttributeInfo
{
    public uint Type;
    public uint Count;
    public uint Size;
}
