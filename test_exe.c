#include <string.h>
#include <stdio.h>

char s1[100000];

int
main(void)
  {
    memset(s1, 1, sizeof(s1));
    fprintf(stderr, "%s\n", "success");
  }
