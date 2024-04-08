namespace Tron.Runtime.Primitives;

/// <summary>
/// A shader object.
/// </summary>
public sealed class Shader
{
    private readonly unsafe CodeGen.Shader* _pointer;

    internal unsafe Shader(CodeGen.Shader* ptr)
    {
        _pointer = ptr;
    }

    /// <summary>
    /// Prints logs of <see cref="Shader"/>.
    /// </summary>
    public void PrintLog()
    {
        unsafe
        {
            CodeGen.Shader.PrintLog(_pointer);
        }
    }
}