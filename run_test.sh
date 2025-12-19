#!/bin/sh -x

clang --target=x86_64-w64-mingw32 -masm=intel mcfgthread/*.c -nostdlib -shared -o libmcfgthread.dll
clang++ --target=x86_64-w64-mingw32 -static test.cpp libmcfgthread.dll -o test.exe
./test.exe

clang --target=arm64ec-w64-mingw32 mcfgthread/*.c -nostdlib -shared -o libmcfgthread.dll -Wl,--gc-sections
./test.exe
