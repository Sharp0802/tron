using System.Runtime.InteropServices;

namespace Tron.Runtime;

/// <summary>
/// Vertex attribute information for VAO (vertex array object) in OpenGL
/// </summary>
[Serializable]
[StructLayout(LayoutKind.Sequential)]
public struct VertexAttributeInfo
{
    /// <summary>
    /// The type of elements in OpenGL enumeration
    /// </summary>
    public uint Type;
    
    /// <summary>
    /// The count of elements
    /// </summary>
    public uint Count;
    
    /// <summary>
    /// The total size of element
    /// </summary>
    public uint Size;
}
