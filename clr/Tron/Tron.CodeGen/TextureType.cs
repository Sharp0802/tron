namespace Tron.CodeGen;

public enum TextureType
{
    UnsignedByte          = 0x1401,
    Byte                  = 0x1400,
    UnsignedShort         = 0x1403,
    Short                 = 0x1402,
    UnsignedInt           = 0x1405,
    Int                   = 0x1404,
    HalfFloat             = 0x140B,
    Float                 = 0x1406,
    UnsignedByte322       = 0x8032,
    UnsignedByte233Rev    = 0x8362,
    UnsignedShort565      = 0x8363,
    UnsignedShort565Rev   = 0x8364,
    UnsignedShort4444     = 0x8033,
    UnsignedShort4444Rev  = 0x8365,
    UnsignedShort5551     = 0x8034,
    UnsignedShort1555Rev  = 0x8366,
    UnsignedInt8888       = 0x8035,
    UnsignedInt8888Rev    = 0x8367,
    UnsignedInt1010102    = 0x8036,
    UnsignedInt2101010Rev = 0x8368,

    Default = UnsignedByte
}