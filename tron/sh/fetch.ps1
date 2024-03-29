$glew     = "https://jaist.dl.sourceforge.net/project/glew/glew/2.1.0/glew-2.1.0-win32.zip"
$glfw3_86 = "https://github.com/glfw/glfw/releases/download/3.3.9/glfw-3.3.9.bin.WIN32.zip"
$glfw3_64 = "https://github.com/glfw/glfw/releases/download/3.3.9/glfw-3.3.9.bin.WIN64.zip"
$libroot  = ".tmp"
$tmproot  = "$libroot/.tmp"

add-type -assemblyname System.IO.Compression.FileSystem

function Ensure {
    param(
        [string]$dir
    )
    mkdir -Force -p "$dir" | out-null
}

function Unzip {
    param(
        [string]$src,
        [string]$dst
    )
    [System.IO.Compression.ZipFile]::ExtractToDirectory("$src", "$dst")
}

function WebRequest {
    param(
        [string]$src,
        [string]$dst
    )
    if (test-path -path "$dst" -pathtype leaf) {
        write-host "skip: $dst" -f Green
    } else {
        invoke-webrequest "$src" -outfile "$dst"
    }
}

ensure $libroot
ensure $tmproot

webrequest $glew     $tmproot/glew.zip
webrequest $glfw3_86 $tmproot/glfw86.zip
webrequest $glfw3_64 $tmproot/glfw64.zip

remove-item -recurse -force $tmproot/glew
remove-item -recurse -force $tmproot/glfw86
remove-item -recurse -force $tmproot/glfw64

unzip $tmproot/glew.zip   $tmproot/glew
unzip $tmproot/glfw86.zip $tmproot/glfw86
unzip $tmproot/glfw64.zip $tmproot/glfw64

remove-item -recurse -force "$libroot/bin/x86/"
remove-item -recurse -force "$libroot/bin/x64/"
remove-item -recurse -force "$libroot/lib/x86/"
remove-item -recurse -force "$libroot/lib/x64/"
remove-item -recurse -force "$libroot/inc/"

ensure "$libroot/bin/x86/"
ensure "$libroot/bin/x64/"
ensure "$libroot/lib/x86/"
ensure "$libroot/lib/x64/"
ensure "$libroot/inc/"

move-item -force $tmproot/glew/*/bin/Release/Win32/glew32.dll "$libroot/bin/x86/glew32.dll"
move-item -force $tmproot/glew/*/lib/Release/Win32/glew32.lib "$libroot/lib/x86/glew32.lib"
move-item -force $tmproot/glew/*/bin/Release/x64/glew32.dll   "$libroot/bin/x64/glew32.dll"
move-item -force $tmproot/glew/*/lib/Release/x64/glew32.lib   "$libroot/lib/x64/glew32.lib"
move-item -force $tmproot/glew/*/include/GL                   "$libroot/inc"

move-item -force $tmproot/glfw64/*/lib-static-ucrt/glfw3.dll    "$libroot/bin/x64/glfw3.dll"
move-item -force $tmproot/glfw64/*/lib-static-ucrt/glfw3dll.lib "$libroot/lib/x64/glfw3.lib"
move-item -force $tmproot/glfw86/*/lib-static-ucrt/glfw3.dll    "$libroot/bin/x86/glfw3.dll"
move-item -force $tmproot/glfw86/*/lib-static-ucrt/glfw3dll.lib "$libroot/lib/x86/glfw3.lib"
move-item -force $tmproot/glfw86/*/include/GLFW                 "$libroot/inc"
