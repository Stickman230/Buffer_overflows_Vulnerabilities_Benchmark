// 64 bit
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


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
  setreuid(geteuid(), geteuid());

  strcpy(cmd, "/bin/ls -l ");

  printf("Path to directory you want to see : ");

  gets(arg);
  Securecheck(arg);

  strcat(cmd, arg);
  system(cmd);

  return 0;
}