# трон

A simple game engine with OpenGL 4.5, written in C++26.

## Requirements

### Runtime

- GPU
  - OpenGL 4.5 compatible \[recommanded\] -or-
  - OpenGL 3.2 core profile compatible \[not tested, not recommanded\]
  - (But, With Mesa and Zink, You can use Vulkan instead of OpenGL)
- CPU
  - x86/x86-64 required

### Building

> [!WARNING]
> Building in Windows is not supported temporarily.
> (Code generator is broken on Windows)

- `clang`, `libclang`
  - Tested on `clang 18.1.3`
- CMake 3.25 or above
- Internet connection (project automatically downloads dependencies)
- \[On Linux\]
  - GLFW3, GLEW, GLM, OpenGL SDK from package manager
- \[On Windows\]
  - Powershell (You don't have to install modern powershell; Tested on 5.1)
  - OpenGL Driver; see [docs](https://www.khronos.org/opengl/wiki/Getting_Started#Windows)

## Thanks to (Dependencies)

- CMake (https://cmake.org/)
- Clang, libclang (https://clang.llvm.org/)
- OpenGL (https://www.opengl.org/)
- GLEW (https://glew.sourceforge.net/)
- GLFW (https://www.glfw.org/)
- STB (https://github.com/nothings/stb)
- .NET (https://github.com/dotnet)
