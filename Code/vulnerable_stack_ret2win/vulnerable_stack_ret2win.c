#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// 32 bit
// PIE	Position Independent Executable	 N 
// RelRO	Read Only relocations	 N 
// NX	Non-Executable Stack	 Y 
// Heap exec	Non-Executable Heap	 Y 
// ASLR	Address Space Layout Randomization	 N 
// SF	Source Fortification	 N 
// SSP	Stack-Smashing Protection	 N
// SRC	Source code access	 Y

void ret2win() {
    setreuid(geteuid(), geteuid());
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