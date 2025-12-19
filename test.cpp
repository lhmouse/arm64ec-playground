#include <stdio.h>

using once_fn = void (void*);
extern "C" __declspec(dllimport) void my_once(int* flag, once_fn* fn, void* arg);

static
void
once_do_it(void* add)
  {
    ::fprintf(stderr, "throwing\n");
    throw 42;
  }

int
main(void)
  {
    ::fprintf(stderr, "main\n");
    try {
      int once = 0;
      ::my_once(&once, once_do_it, (void*) 1);
      __builtin_trap();
    }
    catch(...) { }
    ::fprintf(stderr, "done\n");
  }
