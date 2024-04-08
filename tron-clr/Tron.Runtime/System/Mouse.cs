using GlmSharp;

namespace Tron.Runtime.System;

/// <summary>
/// A manager class for mouse input.
/// </summary>
public static class Mouse
{
    /// <summary>
    /// Gets position of mouse.
    /// </summary>
    public static vec2 Position => CodeGen.Input.GetMouseMove();

    /// <summary>
    /// Gets scroll value of mouse.
    /// </summary>
    public static float Scroll => CodeGen.Input.GetScroll();

    /// <summary>
    /// Gets whether specific button is pressed on mouse.
    /// </summary>
    /// <param name="button">The code of button</param>
    /// <returns>True if button is pressed; otherwise, false</returns>
    public static bool GetButton(MouseButton button)
    {
        return CodeGen.Input.GetMouseClick((int)button);
    }
}