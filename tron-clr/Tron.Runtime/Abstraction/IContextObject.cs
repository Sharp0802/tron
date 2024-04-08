namespace Tron.Runtime.Abstraction;

/// <summary>
/// An object required to be bound on context.
/// </summary>
public interface IContextObject
{
    /// <summary>
    /// Binds object to current context.
    /// </summary>
    public void Use();
}