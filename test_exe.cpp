#include <vector>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN  1
#include <windows.h>

__declspec(dllimport)
const char*
test_dll_arch();

const char*
test_exe_arch()
  {
    return
#ifdef _M_IX86
      " x86-32"
#endif
#ifdef _M_AMD64
      " x86-64"
#endif
#ifdef _M_ARM64
      " ARM64"
#endif
#ifdef _M_ARM64EC
      " ARM64EC"
#endif
      ;
  }

std::vector<void*> v1(1);

int
main(void)
  {
    ::fprintf(stderr, "test_exe_arch = %s\n", test_exe_arch());
    ::fprintf(stderr, "test_dll_arch = %s\n", test_dll_arch());
    ::fprintf(stderr, "v1.size       = %zd\n", v1.size());
  }
