using CsInterop;
using Tron.Runtime.Abstraction;
using Tron.Runtime.CodeGen;
using Tron.Runtime.InteropServices;

namespace Tron.Runtime.Primitives;

/// <summary>
/// A mesh object.
/// </summary>
public class Mesh : IContextObject, IDrawable
{
    private readonly unsafe CodeGen.Mesh* _pointer;

    /// <summary>
    /// Creates new mesh object.
    /// </summary>
    /// <param name="attributes">Vertex attributes to apply</param>
    /// <param name="usage">The usage of mesh object</param>
    public Mesh(
        Span<VertexAttributeInfo> attributes,
        VertexBufferUsage         usage = VertexBufferUsage.DEFAULT_DRAW)
    {
        unsafe
        {
            _pointer = NativeMemory.Alloc<CodeGen.Mesh>();
            CodeGen.Mesh.__ctor__(_pointer, new InitializerList<VertexAttributeInfo>(attributes), usage);

            Indices = new IndexBuffer(CodeGen.Mesh.get__EBO(_pointer));
            Vertices = new VertexBuffer(CodeGen.Mesh.get__VBO(_pointer));
        }
    }

    /// <inheritdoc />
    public void Use()
    {
        unsafe
        {
            CodeGen.Mesh.Bind(_pointer);
        }
    }

    /// <inheritdoc />
    public void Draw()
    {
        unsafe
        {
            CodeGen.Mesh.Draw(_pointer);
        }
    }

    /// <summary>
    /// Indices buffer of <see cref="Mesh"/>.
    /// </summary>
    public IndexBuffer  Indices { get; }

    /// <summary>
    /// Vertices buffer of <see cref="Mesh"/>.
    /// </summary>
    public VertexBuffer Vertices { get; }

    ~Mesh()
    {
        unsafe
        {
            NativeMemory.Free(_pointer);
        }
    }
}