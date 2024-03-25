namespace Tron.CodeGen;

public enum TextureFilter
{
    Nearest              = 0x2600,
    Linear               = 0x2601,
    NearestMipmapNearest = 0x2700,
    LinearMipmapNearest  = 0x2701,
    NearestMipmapLinear  = 0x2702,
    LinearMipmapLinear   = 0x2703,

    Default = Nearest
}