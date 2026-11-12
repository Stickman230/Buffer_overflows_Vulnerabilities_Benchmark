// 32 bit
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void function1() {
    system("/bin/bash");
}

void function2() {
    printf("Hello, welcome to my new program !\n");
}

void main()
{ 
    int var;
    void (*func)()=function2;
    char buf[128];
    fgets(buf,133,stdin);
    func();
}