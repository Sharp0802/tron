namespace Tron.CodeGen;

public enum TextureWrap
{
    Repeat            = 0x2901,
    ClampToEdge       = 0x812F,
    ClampToBorder     = 0x812D,
    MirroredRepeat    = 0x8370,
    MirrorClampToEdge = 0x8743,

    Default = Repeat
}