namespace Tron.Runtime.Primitives;

public class Shader
{
    private readonly unsafe CodeGen.Shader* _pointer;

    internal unsafe Shader(CodeGen.Shader* ptr)
    {
        _pointer = ptr;
    }

    public void PrintLog()
    {
        unsafe
        {
            CodeGen.Shader.PrintLog(_pointer);
        }
    }
}