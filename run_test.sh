#!/bin/sh -x

clang --target=x86_64-w64-mingw32 -masm=intel -D__MCF_IN_DLL -I. -Imcfgthread mcfgthread/*.c -nostdlib -lkernel32 -lntdll -shared -o libmcfgthread.dll
clang++ --target=x86_64-w64-mingw32 -static -I. -Imcfgthread test.cpp libmcfgthread.dll -o test.exe
./test.exe

clang --target=arm64ec-w64-mingw32 -D__MCF_IN_DLL -I. -Imcfgthread mcfgthread/*.c -nostdlib -lkernel32 -lntdll -shared -o libmcfgthread.dll -Wl,--gc-sections
./test.exe
