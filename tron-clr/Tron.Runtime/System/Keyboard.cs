namespace Tron.Runtime.System;

/// <summary>
/// A manager class of keyboard input.
/// </summary>
public static class Keyboard
{
    /// <summary>
    /// Gets whether specific key is pressed on keyboard.
    /// </summary>
    /// <param name="key">The code of key</param>
    /// <returns>True if key is pressed; otherwise, false</returns>
    public static bool GetKey(KeyCode key)
    {
        return CodeGen.Input.GetKey((int)key);
    }
}