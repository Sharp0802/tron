using GlmSharp;

namespace Tron.Runtime.System;

public enum MouseButton
{
    _1     = 0,
    _2     = 1,
    _3     = 2,
    _4     = 3,
    _5     = 4,
    _6     = 5,
    _7     = 6,
    _8     = 7,
    Last   = _8,
    Left   = _1,
    Right  = _2,
    Middle = _3
}

public static class Mouse
{
    public static vec2 Position => CodeGen.Input.GetMouseMove();

    public static float Scroll => CodeGen.Input.GetScroll();

    public static bool GetButton(MouseButton button)
    {
        return CodeGen.Input.GetMouseClick((int)button);
    }
}