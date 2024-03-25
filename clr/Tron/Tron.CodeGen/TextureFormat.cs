namespace Tron.CodeGen;

public enum TextureFormat : uint
{
    R              = 0x1903,
    RG             = 0x8227,
    RGB            = 0x1907,
    RGBA           = 0x1908,
    BGR            = 0x80E0,
    BGRA           = 0x80E1,
    StencilIndex   = 0x1901,
    DepthComponent = 0x1902,
    DepthStencil   = 0x84F9,

    Default = uint.MaxValue
}