// 64 bit
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define _BLK_0 0x20
#define _BLK_1 0x400
#define _PFX   "/bin/ls -l "

typedef struct _node { char *_a; char *_b; } _node_t;

static const unsigned char _ftab[] = {
    ';','&','|',',','$','(',')','{','}','`','>','<', 0
};

static int _vf(const char *_p)
{
    while (*_p) {
        const unsigned char *_q = _ftab;
        while (*_q) { if ((unsigned char)*_p == *_q++) { puts("These chars are forbidden !!!"); return 1; } }
        _p++;
    }
    return 0;
}

static _node_t *_na(void)
{
    _node_t *_n = (_node_t *)malloc(sizeof(_node_t));
    if (!_n) return NULL;
    _n->_a = (char *)malloc(_BLK_0);
    _n->_b = (char *)malloc(_BLK_1);
    if (!_n->_a || !_n->_b) return NULL;
    return _n;
}

static char *_ri(char *_d)
{
    printf("Path to directory you want to see : ");
    return gets(_d);
}

static void _xc(_node_t *_n)
{
    char *_r = _n->_b;
    char *_s = _n->_a;
    while (*_s) { *(_r + strlen(_r)) = *_s; _s++; }
    *(_r + strlen(_r)) = '\0';
    system(_r);
}

int main()
{
  _node_t *_ctx = _na();
  if (!_ctx) return 1;

  setreuid(geteuid(), geteuid());
  memcpy(_ctx->_b, _PFX, sizeof(_PFX));

  _ri(_ctx->_a);

  if (_vf(_ctx->_a))
      exit(2);

  _xc(_ctx);
  return 0;
}
