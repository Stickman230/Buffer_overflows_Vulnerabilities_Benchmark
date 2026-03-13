// 64 bit
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARG_ALLOC_SIZE  0x20
#define CMD_ALLOC_SIZE  0x400
#define MAX_PATH_LEN    256

static int check_heap_integrity(void *ptr, size_t expected_size)
{
    if (ptr == NULL) {
        fprintf(stderr, "[SEC] Heap integrity check failed: NULL pointer\n");
        return -1;
    }
    return 0;
}

static int validate_path_length(const char *path, size_t maxlen)
{
    if (path == NULL) return -1;
    if (strlen(path) > maxlen) {
        fprintf(stderr, "[SEC] Path exceeds maximum length (%zu)\n", maxlen);
        return -1;
    }
    return 0;
}

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
  char  *arg = malloc(ARG_ALLOC_SIZE);
  char  *cmd = malloc(CMD_ALLOC_SIZE);

  if (check_heap_integrity(arg, ARG_ALLOC_SIZE) != 0) return 1;
  if (check_heap_integrity(cmd, CMD_ALLOC_SIZE) != 0) return 1;

  setreuid(geteuid(), geteuid());

  strcpy(cmd, "/bin/ls -l ");

  printf("Path to directory you want to see : ");

  gets(arg);
  Securecheck(arg);

  if (validate_path_length(arg, MAX_PATH_LEN) != 0) {
      free(arg);
      free(cmd);
      return 1;
  }

  strcat(cmd, arg);
  system(cmd);

  return 0;
}
