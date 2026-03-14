// 32 bit
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void spawnshell() {
    system("/bin/bash");
}

void welcome() {
    printf("Hello, welcome to my new program !\n");
}

void main()
{ 
    int var;
    void (*func)()=welcome;
    char buf[128];
    fgets(buf,133,stdin);
    func();
}