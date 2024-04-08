namespace Tron.Runtime;

/// <summary>
/// An atomic object of the engine that acts as bridge between managed world with unmanaged wild.
/// </summary>
public abstract class CObject
{
    /// <summary>
    /// An event invoked right after object initialized on engine (before managed .ctor invoked)
    /// </summary>
    public abstract void OnCreated();
    
    /// <summary>
    /// An event invoked when first frame after object enabled.
    /// </summary>
    public abstract void OnEnabled();
    
    /// <summary>
    /// An event invoked when every frame.
    /// </summary>
    /// <param name="delta">Elapsed seconds after last frame (in sec)</param>
    public abstract void OnUpdated(float delta);
    
    /// <summary>
    /// An event invoked when first frame after object disabled.
    /// </summary>
    public abstract void OnDisabled();
    
    /// <summary>
    /// An event invoked when first frame object destroyed (disposed) on unmanaged wild.
    /// </summary>
    public abstract void OnDestroyed();
}