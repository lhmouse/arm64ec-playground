#include "mcfgthread/gthr_aux.h"
#include <stdio.h>
#include <stdlib.h>

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
      ::_MCF_once once = {};
      ::__MCF_gthr_call_once_seh(&once, once_do_it, (void*) 1);
      ::abort();
    }
    catch(...) { }
    ::fprintf(stderr, "done\n");
  }
