// 32 Bit
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_INPUT_LEN 40

static int validate_input_bounds(const char *data, size_t maxlen)
{
    if (data == NULL) return -1;
    size_t len = strlen(data);
    if (len > maxlen) {
        fprintf(stderr, "[SEC] Input exceeds maximum allowed length (%zu > %zu)\n", len, maxlen);
        return -1;
    }
    return 0;
}

static int sanitize_input(char *data)
{
    if (data == NULL) return -1;
    char *newline = strchr(data, '\n');
    if (newline) *newline = '\0';
    return 0;
}

int main()
{
  int var;
  int mycheck = 0x87654321;
  char buf[40];

  fgets(buf,45,stdin);

  sanitize_input(buf);

  printf("\n[buf]: %s\n", buf);
  printf("[check] %p\n", mycheck);

  if (mycheck == 0xdeadbeef)
   {
     if (validate_input_bounds(buf, MAX_INPUT_LEN) != 0) {
         fprintf(stderr, "[SEC] Blocked: input validation failed\n");
         return 1;
     }
     setreuid(geteuid(), geteuid());
     system("/bin/bash");
   }
   return 0;
}
