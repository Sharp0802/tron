# трон-cg

C# binding generator with `libclang` and `libomp`

## Usage

```shell
tron-cg <root-of-project> [<include-directories>...]
```

For example:

```shell
tron-cg <dir> "/usr/lib/gcc/x86_64-pc-linux-gnu/13.2.1/include/" "/usr/include"
```

Then, `cg.g.cs` will be generated at `<dir>/tron-clr/cg.g.cs`.
