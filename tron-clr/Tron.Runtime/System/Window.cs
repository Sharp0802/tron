using NativeMemory = Tron.Runtime.InteropServices.NativeMemory;

namespace Tron.Runtime.System;

/// <summary>
/// A system object to control <see cref="Window"/>.
/// </summary>
public sealed class Window
{
    private static          Window?         _current;
    private readonly unsafe CodeGen.Window* _pointer;

    /// <summary>
    /// Initializes <see cref="Window"/> class.
    /// </summary>
    public Window()
    {
        unsafe
        {
            _pointer = NativeMemory.Alloc<CodeGen.Window>();
            CodeGen.Window.__ctor__(_pointer);
        }
    }

    ~Window()
    {
        unsafe
        {        
            NativeMemory.Free(_pointer);
        }
    }

    /// <summary>
    /// Gets width of the <see cref="Window"/>.
    /// </summary>
    public int Width
    {
        get
        {
            unsafe
            {
                return CodeGen.Window.get__Width(_pointer);
            }
        }
    }

    /// <summary>
    /// Gets height of the <see cref="Window"/>.
    /// </summary>
    public int Height
    {
        get
        {
            unsafe
            {
                return CodeGen.Window.get__Height(_pointer);
            }
        }
    }

    /// <summary>
    /// Gets or sets title of the <see cref="Window"/>.
    /// </summary>
    public string Title
    {
        get
        {
            unsafe
            {
                return CodeGen.Window.get__Title(_pointer).ToString();
            }
        }
        set
        {
            unsafe
            {
                CodeGen.Window.set__Title(_pointer, value);
            }
        }
    }

    /// <summary>
    /// Gets or sets whether the <see cref="Window"/> should be closed at next frame.
    /// </summary>
    public bool ShouldClose
    {
        get
        {
            unsafe
            {
                return CodeGen.Window.get__ShouldClose(_pointer);
            }
        }
        set
        {
            unsafe
            {
                CodeGen.Window.set__ShouldClose(_pointer, value);
            }
        }
    }

    /// <summary>
    /// Uses this <see cref="Window"/> in current context.
    /// </summary>
    public void Use()
    {
        unsafe
        {
            CodeGen.Window.Bind(_pointer);
            _current = this;
        }
    }

    /// <summary>
    /// Draws and refreshes <see cref="Window"/>.
    /// </summary>
    public void Draw()
    {
        unsafe
        {
            CodeGen.Window.Draw(_pointer);
        }
    }

    /// <summary>
    /// Sets input mode of <see cref="Window"/>.
    /// </summary>
    /// <param name="key"></param>
    /// <param name="mode"></param>
    /// <remarks>
    /// <seealso href="https://www.glfw.org/docs/latest/group__input.html#gaa92336e173da9c8834558b54ee80563b"/>
    /// </remarks>
    public void SetInputMode(int key, int mode)
    {
        unsafe
        {
            CodeGen.Window.SetInputMode(_pointer, key, mode);
        }
    }

    /// <summary>
    /// Gets current bound <see cref="Window"/> object.
    /// </summary>
    public static Window? Current => _current;
}