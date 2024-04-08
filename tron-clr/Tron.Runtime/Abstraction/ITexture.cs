namespace Tron.Runtime.Abstraction;

public interface ITexture : IDisposable, IContextObject
{
    internal IntPtr Pointer { get; }
}