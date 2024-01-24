#include "asm.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main() {
  asm_jmp_buf buf;
  int r = asm_setjmp(buf);
  if (r == 0) {
    assert(asm_add(1234, 5678) == 6912);
    assert(asm_popcnt(0x0123456789abcdefULL) == 32); 
    // TODO: add more tests here.

    char *a="I love pa!";
    char dest[10] = {'\0'};
    asm_memcpy(dest, a, 3);
    //printf("a=%s\n",a);
    //printf("dest=%s\n",dest);
    //assert(strncmp(dest, a, 3)==0);

    asm_longjmp(buf, 123);
    //printf("r=%d\n",r);
  } else {
    assert(r == 123);
    printf("PASSED.\n");
  }
}
