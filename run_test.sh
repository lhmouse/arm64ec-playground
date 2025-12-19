#!/bin/sh -x

clang --target=x86_64-w64-mingw32 -masm=intel m/*.c -nostdlib -shared -o libm.dll
clang++ --target=x86_64-w64-mingw32 -static test.cpp libm.dll -o test.exe
./test.exe

clang --target=arm64ec-w64-mingw32 m/*.c -nostdlib -shared -o libm.dll -Wl,--gc-sections
./test.exe
