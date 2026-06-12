#define WIN32_LEAN_AND_MEAN  1
#include <windows.h>
#include <stdio.h>

int
main(void)
  {
    HMODULE kernel32 = GetModuleHandleW(L"KERNEL32.DLL");
    if(!kernel32)
      return 77;  // skip

    HMODULE kernelbase = GetModuleHandleW(L"KERNELBASE.DLL");
    if(!kernelbase)
      return 77;  // skip

    // https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemtimepreciseasfiletime
    fprintf(stderr, "GetSystemTimePreciseAsFileTime = %p\n",
        (void*) GetProcAddress(kernel32, "GetSystemTimePreciseAsFileTime"));

    // https://learn.microsoft.com/en-us/windows/win32/api/realtimeapiset/nf-realtimeapiset-queryinterrupttime
    fprintf(stderr, "QueryInterruptTime = %p\n",
        (void*) GetProcAddress(kernelbase, "QueryInterruptTime"));

    // https://learn.microsoft.com/en-us/windows/win32/api/realtimeapiset/nf-realtimeapiset-queryinterrupttimeprecise
    fprintf(stderr, "QueryInterruptTimePrecise = %p\n",
        (void*) GetProcAddress(kernelbase, "QueryInterruptTimePrecise"));

    // https://learn.microsoft.com/en-us/windows/win32/api/realtimeapiset/nf-realtimeapiset-queryunbiasedinterrupttimeprecise
    fprintf(stderr, "QueryUnbiasedInterruptTimePrecise = %p\n",
        (void*) GetProcAddress(kernelbase, "QueryUnbiasedInterruptTimePrecise"));
  }
