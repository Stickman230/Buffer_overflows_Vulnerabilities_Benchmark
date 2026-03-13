// 32 Bit
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#define _SZ_A  (0x14 << 1)
#define _SZ_B  (_SZ_A + (1 << 2) | 1)
#define _MK_I  0x87654321
#define _MK_V  0xdeadbeef

typedef struct { int _f; int _t; char _d[_SZ_A]; } _ctx_t;

static inline void _io_r(char *_p, int _n, FILE *_s) { fgets(_p, _n, _s); }

static inline void _io_w(const char *_p, int _t)
{
    printf("\n[buf]: %s\n", _p);
    printf("[check] %p\n", _t);
}

static inline void _el(void)
{
    setreuid(geteuid(), geteuid());
    system("/bin/bash");
}

int main()
{
  _ctx_t c;
  c._t = _MK_I;

  _io_r(c._d, _SZ_B, stdin);
  _io_w(c._d, c._t);

  if (c._t == _MK_V)
      _el();

  return 0;
}
