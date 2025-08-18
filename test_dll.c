#if defined _WIN64 || defined _LP64
typedef unsigned long long size_t;
#else
typedef unsigned int size_t;
#endif

__declspec(dllimport)
void
__stdcall
RtlFillMemory(void* dst, size_t n, int ch);

__declspec(dllimport)
void
__stdcall
RtlMoveMemory(void* dst, const void* src, size_t);

///////////////////////////////////////////////////////////

__declspec(dllexport)
void*
memset(void* dst, int ch, size_t n)
  {
    RtlFillMemory(dst, n, ch);
    return dst;
  }

__declspec(dllexport)
void*
memcpy(void* dst, const void* src, size_t n)
  {
    RtlMoveMemory(dst, src, n);
    return dst;
  }

__declspec(dllexport)
void*
memmove(void* dst, const void* src, size_t n)
  {
    RtlMoveMemory(dst, src, n);
    return dst;
  }
