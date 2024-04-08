namespace Tron.Runtime.System;

public static class Keyboard
{
    public static bool GetKey(KeyCode key)
    {
        return CodeGen.Input.GetKey((int)key);
    }
}