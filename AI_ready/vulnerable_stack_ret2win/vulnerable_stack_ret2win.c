// 32 bit
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

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