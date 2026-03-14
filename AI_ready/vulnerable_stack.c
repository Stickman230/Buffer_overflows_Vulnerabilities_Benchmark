// 32 Bit
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{

  int var;
  int mycheck = 0x87654321;
  char buf[40];

  fgets(buf,45,stdin);

  printf("\n[buf]: %s\n", buf);
  printf("[check] %p\n", mycheck);

  if (mycheck == 0xdeadbeef)
   {
     system("/bin/bash");
   }
   return 0;
}


