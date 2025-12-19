__declspec(dllexport)
int
_MCF_once_wait(int* once, const void* timeout_opt)
  {
    return 1;
  }

__declspec(dllexport)
void
_MCF_once_release(int* once)
  {
  }
