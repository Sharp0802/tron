# трон

A simple game engine with OpenGL ES 3.1 (4.5 for Desktops), written in C++26 and C# 12.

## Requirements

### Runtime

- GPU
  - OpenGL ES 3.1 Compatible (4.5 for Desktops)
- CPU
  - x86-64 required
- Dependencies
  - .NET 8.0 for above (tested on .NET 8.0.103)

### Building

> [!WARNING]
> Manuals for building project is on WIP state.

|                 Dep | Version                         |                                                Misc                                                |
|--------------------:|:--------------------------------|:--------------------------------------------------------------------------------------------------:|
|               CMake | 3.25 or above; tested on 3.25   |                                                                                                    |
|     GNU C++ Library | tested on 13.2.1                |                                                                                                    |
| Internet Connection |                                 |                         project automatically downloads some dependencies                          |
|          LLVM/Clang | 18.1 or above; tested on 18.1.3 |                                             `libclang`                                             |
|         LLVM/OpenMP | 18.1 or above; tested on 18.1.3 |                                              `libomp`                                              |
|            .NET SDK | 8.0 or above; tested on 8.0.103 |                                                                                                    |
|    GLFW3, GLEW, GLM |                                 | \[**Windows**: You don't have to install these manually. Project automatically downloads things!\] |
|          OpenGL SDK |                                 |                                                                                                    |

## Thanks to

- CMake (https://cmake.org/)
- GLEW (https://glew.sourceforge.net/)
- GLFW (https://www.glfw.org/)
- LLVM/Clang (https://clang.llvm.org/)
- LLVM/OpenMP (https://openmp.llvm.org/)
- .NET (https://github.com/dotnet)
- OpenGL (https://www.opengl.org/)
- STB (https://github.com/nothings/stb)
