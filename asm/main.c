#include "asm.h"
#include <assert.h>
#include <stdio.h>

int main() {
  asm_jmp_buf buf;
  int r = 0;
  // int r = setjmp(buf);
  if (r == 0) {
    assert(asm_add(1234, 5678) == 6912);
    assert(asm_popcnt(0x0123456789abcdefULL) == 32);
    char test[10];
    char s[]="hello!";
    asm_memcpy(test, s, 6);
    printf("%s = %s\n", s, test);
    assert(test[0] == 'h');
    printf("Add & Popcnt done.\n");
    // TODO: add more tests here.
    // longjmp(buf, 123);
  } else {
    assert(r == 123);
    printf("PASSED.\n");
  }
}
