#include <windows.h>
#include <stdio.h>

using vfni = void (int);

extern "C"
EXCEPTION_DISPOSITION
__cdecl
handler(EXCEPTION_RECORD* er, void* frm, CONTEXT* ctx, void* disp)
  {
    ::printf("handler: 0x%.8lx (0x%.2lx)\n", er->ExceptionCode, er->ExceptionFlags);
    return ExceptionContinueSearch;
  }

__attribute__((__noinline__))
void
use_fn(vfni* fn, int x)
  {
#ifdef __i386__
    DWORD seh_record[2];
    seh_record[0] = *(unsigned long __seg_fs*) 0;
    seh_record[1] = (DWORD) handler;
    *(unsigned long __seg_fs*) 0 = (DWORD) seh_record;
    __asm__ (".safeseh _handler");
#else
    __asm__ (".seh_handler handler, @except");
#endif

    ::printf("calling fn %p\n", fn);
    (*fn) (x);
    ::printf("returned from fn %p\n", fn);

#ifdef __i386__
    *(unsigned long __seg_fs*) 0 = seh_record[0];
#endif
  }

int
main(void)
  {
    try {
      use_fn(
        [](int x) {
          ::printf("throwing %d\n", x);
          throw x;
        },
        12345);
    }
    catch(int x) {
      ::printf("caught %d\n", x);
    }
  }
