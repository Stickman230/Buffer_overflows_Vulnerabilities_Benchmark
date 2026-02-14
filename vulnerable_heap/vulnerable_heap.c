#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 64 bit
// PIE	  Position Independent Executable	 Y 
// RelRO	  Read Only relocations	 Y
// NX	  Non-Executable Stack	 Y 
// Heap exec	  Non-Executable Heap	 Y 
// ASLR	  Address Space Layout Randomization	 Y
// SF	  Source Fortification	 N
// SSP	  Stack-Smashing Protection	 N
// SRC	  Source code access	 Y


void    checkArg(const char *a)
{
  while (*a)
    {
      if (   (*a == ';')
          || (*a == '&')
          || (*a == '|')
          || (*a == ',')
          || (*a == '$')
          || (*a == '(')
          || (*a == ')')
          || (*a == '{')
          || (*a == '}')
          || (*a == '`')
          || (*a == '>')
          || (*a == '<') ) {
        puts("These chars are forbidden !!!");
        exit(2);
      }
        a++;
    }
}

int     main()
{
  char  *arg = malloc(0x20);
  char  *cmd = malloc(0x400);
  setreuid(geteuid(), geteuid());

  strcpy(cmd, "/bin/ls -l ");

  printf("Path to directory you want to see : ");

  gets(arg);
  checkArg(arg);

  strcat(cmd, arg);
  system(cmd);

  return 0;
}