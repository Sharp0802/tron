namespace Tron.Runtime.Abstraction;

/// <summary>
/// A texture abstraction.
/// </summary>
public abstract class Texture : IDisposable, IContextObject
{
    internal abstract IntPtr Pointer { get; }

    /// <inheritdoc />
    public abstract void Use();

    /// <inheritdoc />
    public abstract void Dispose();
}