#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

// 32 Bit
// PIE	  Position Independent Executable	 N 
// RelRO	  Read Only relocations	 N
// NX	  Non-Executable Stack	 N 
// Heap exec	  Non-Executable Heap	 N 
// ASLR	  Address Space Layout Randomization	 N
// SF	  Source Fortification	 N
// SSP	  Stack-Smashing Protection	 N
// SRC	  Source code access	 Y

// gcc -m32 -O0 -g vulnerable_stack.c -o vulnerable_stack \
// -fno-stack-protector \
// -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 \
// -z execstack \
// -no-pie -fno-pie \
// -Wl,-z,norelro

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
     setreuid(geteuid(), geteuid());
     system("/bin/bash");
   }
   return 0;
}


