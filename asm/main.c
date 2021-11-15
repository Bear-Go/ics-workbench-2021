#include "asm.h"
#include <assert.h>
#include <stdio.h>

int main() {
  asm_jmp_buf buf;
  int r = asm_setjmp(buf);
  if (r == 0) {
    assert(asm_add(1234, 5678) == 6912);
    assert(asm_popcnt(13) == 3);
    char test[10];
    char s[]="hello!";
    asm_memcpy(test, s, 6);
    printf("%s = %s\n", s, test);
    assert(test[0] == 'h');
    printf("Add & Popcnt done.\n");
    // TODO: add more tests here.
    asm_longjmp(buf, 123);
  } else {
    printf("this!\n");
    printf("%d\n", r);
    assert(r == 123);
    printf("PASSED.\n");
  }
}
