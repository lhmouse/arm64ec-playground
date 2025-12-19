int _MCF_once_wait(int* once, const void* timeout_opt);
typedef void my_once_fn(void*);

__declspec(dllexport)
void
my_once(int* once, my_once_fn* init_proc, void* arg)
  {
    int err = _MCF_once_wait(once, 0);
    if(err == 0)
      return;

    init_proc(arg);
  }
