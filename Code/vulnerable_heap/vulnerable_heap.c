#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 64 bit
// PIE	  Position Independent Executable	 Y 
// RelRO	  Read Only relocations	 Y
// NX	  Non-Executable Stack	 Y 
// Heap exec	  Non-Executable Heap	 Y 
// ASLR	  Address Space Layout Randomization	 Y
// SF	  Source Fortification	 N
// SSP	  Stack-Smashing Protection	 N
// SRC	  Source code access	 Y

//gcc -m64 -fPIE -pie -Wl,-z,relro,-z,now -Wl,-z,noexecstack -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -fno-stack-protector -o vulnerable_heap vulnerable_heap.c

void Securecheck(const char *a)
{
    for (const unsigned char *p = (const unsigned char *)a; *p; ++p)
    {
        switch (*p)
        {
            case ';': case '&': case '|': case ',': case '$':
            case '(': case ')': case '{': case '}': case '`':
            case '>': case '<':
                puts("These chars are forbidden !!!");
                exit(2);
        }
    }
}

int     main()
{
  char  *arg = malloc(0x20);
  char  *cmd = malloc(0x400);

  strcpy(cmd, "/bin/ls -l ");

  printf("Path to directory you want to see : ");

  gets(arg);
  Securecheck(arg);

  strcat(cmd, arg);
  system(cmd);

  return 0;
}