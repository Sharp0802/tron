namespace Tron.Runtime.System;

/// <summary>
/// A manager class for input-system.
/// </summary>
public static class Input
{
    /// <summary>
    /// Poll events from native input-system.
    /// </summary>
    public static void Poll()
    {
        CodeGen.Input.Poll();
    }
}