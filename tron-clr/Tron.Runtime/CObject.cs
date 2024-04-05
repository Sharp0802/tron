namespace Tron.Runtime;

public abstract class CObject
{
    public abstract void OnCreated();
    public abstract void OnEnabled();
    public abstract void OnUpdated(float delta);
    public abstract void OnDisabled();
    public abstract void OnDestroyed();
}