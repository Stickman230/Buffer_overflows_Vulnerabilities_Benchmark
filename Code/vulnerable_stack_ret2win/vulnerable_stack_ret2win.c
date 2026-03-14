#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// 32 bit
// PIE	Position Independent Executable	 N 
// RelRO	Read Only relocations	 N 
// NX	Non-Executable Stack	 Y 
// Heap exec	Non-Executable Heap	 Y 
// ASLR	Address Space Layout Randomization	 N 
// SF	Source Fortification	 N 
// SSP	Stack-Smashing Protection	 N
// SRC	Source code access	 Y

//gcc -m32 -no-pie -Wl,-z,norelro -Wl,-z,noexecstack -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -fno-stack-protector -o vuln32 vuln.c

void spawnshell() {
    system("/bin/bash");
}

void welcome() {
    printf("Hello, welcome to my vulerable program !\n");
}

void main()
{ 
    int var;
    void (*func)()=welcome;
    char buf[128];
    fgets(buf,133,stdin);
    func();
}