#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// gcc -m32 -fno-stack-protector -z execstack -no-pie -o simple_overflow simple_overflow.c

#define MAX_LEN 256

int main(int argc, char **argv) {
    FILE *fp;
    unsigned int len;
    char *buf, *cmd;
    
    if(argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }
    
    fp = fopen(argv[1], "r");
    if(!fp) {
        perror("fopen");
        exit(1);
    }
    
    // VULNERABLE: Read unsigned int length
    fread(&len, 4, 1, fp);
    printf("[+] Length: %u\n", len);
    
    // INTEGER OVERFLOW: UINT_MAX → malloc(0) = tiny buffer
    buf = malloc(len);
    
    // OVERFLOW: Read huge amount into tiny heap buffer
    fread(buf, 1, len, fp);
    
    // DANGEROUS: Execute corrupted buffer as command!
    printf("[+] Executing: %s\n", buf);
    cmd = malloc(256);
    strcpy(cmd, buf);  // Copy overflowed data
    system(cmd);       // COMMAND EXECUTION!
    
    free(buf);
    free(cmd);
    fclose(fp);
    return 0;
}