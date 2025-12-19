#!/bin/sh -x

clang --target=x86_64-w64-mingw32 -masm=intel m/*.c -nostdlib -shared -o libm.dll
clang++ --target=x86_64-w64-mingw32 -static test.cpp libm.dll -o test.exe
./test.exe  # good

clang --target=arm64ec-pc-windows-msvc m/gthr_aux.c -S -o gthr_aux.s
clang --target=arm64ec-pc-windows-msvc gthr_aux.s -c
clang --target=arm64ec-w64-mingw32 gthr_aux.o m/globals.c m/once.c -nostdlib -shared -o libm.dll -Wl,--gc-sections
./test.exe  # good

clang --target=arm64ec-pc-windows-msvc m/gthr_aux.c -S -o gthr_aux.s
clang --target=arm64ec-w64-mingw32 gthr_aux.s -c
clang --target=arm64ec-w64-mingw32 gthr_aux.o m/globals.c m/once.c -nostdlib -shared -o libm.dll -Wl,--gc-sections
./test.exe  # bad
