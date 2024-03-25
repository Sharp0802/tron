using System.Runtime.InteropServices;
using GlmSharp;

namespace Tron.CodeGen;

public struct VertexAttribute
{
    [StructLayout(LayoutKind.Explicit)]
    private struct VertexAttributeImpl(GLenum type, int size, int count)
    {
        [FieldOffset(0)] public VertexAttributeInfo Info;

        [FieldOffset(0)] public GLenum Type  = type;
        [FieldOffset(4)] public int    Size  = size;
        [FieldOffset(8)] public int    Count = count;
    }

    private static readonly Dictionary<Type, VertexAttributeImpl> Impl = new()
    {
        { typeof(int), new VertexAttributeImpl(GLenum.Int, sizeof(int), 1) },
        { typeof(float), new VertexAttributeImpl(GLenum.Float, sizeof(float), 1) },
        
        
        { typeof(vec2), new VertexAttributeImpl(GLenum.Float, Marshal.SizeOf<vec2>(), 2) },
        { typeof(vec3), new VertexAttributeImpl(GLenum.Float, Marshal.SizeOf<vec3>(), 3) },
        { typeof(vec4), new VertexAttributeImpl(GLenum.Float, Marshal.SizeOf<vec4>(), 4) },
        
        { typeof(mat2), new VertexAttributeImpl(GLenum.Float, Marshal.SizeOf<mat2>(), 2*2) },
        { typeof(mat3), new VertexAttributeImpl(GLenum.Float, Marshal.SizeOf<mat3>(), 3*3) },
        { typeof(mat4), new VertexAttributeImpl(GLenum.Float, Marshal.SizeOf<mat4>(), 4*4) }
    };

    public static VertexAttributeInfo Create<T>()
    {
        return Impl[typeof(T)].Info;
    }
}