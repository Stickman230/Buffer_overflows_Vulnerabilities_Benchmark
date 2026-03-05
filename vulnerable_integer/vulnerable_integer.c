#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char **argv) {
    uint32_t len;

    if(argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    FILE *fp = fopen(argv[1], "rb"); 

    fread(&len, sizeof(uint32_t), 1, fp);

    char *buf = (char *)malloc(len + 1); 

    if (buf == NULL) return 1;

    fread(buf, 1, len, fp); 
    
    buf[len] = '\0';
    printf("Data read successfully.\n");

    free(buf);
    fclose(fp);
    return 0;
}