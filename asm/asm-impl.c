#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
  asm(
    "addq %1, %0\n\t"
    : "+r"(a)
    : "r"(b)
    );
  return a;
}

int asm_popcnt(uint64_t x) {
  int cnt;
  asm(
    "movl $0, %%eax\n\t"
    "movl $0, %%ecx\n\t"
    "inside:\n\t"
    "cmpl $64, %%ecx;"
    "jge outside;"
    "movq %%rbx, %%rdx;"
    "and $1,%%rdx;"
    "addl %%edx,%%eax;"
    "incl %%ecx;"
    "shrq $1,%%rbx;"
    "jmp inside;"
    "outside:"
    :"=a"(cnt)
    :"b"(x)
  );
  // int s = 0;
  // for (int i = 0; i < 64; i++) {
  //   if ((x >> i) & 1) s++;
  // }
  // return s;
  return cnt;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  return memcpy(dest, src, n);
  // return dest;
}

int asm_setjmp(asm_jmp_buf env) {
  return 0;
  // return setjmp(env);
}

void asm_longjmp(asm_jmp_buf env, int val) {
  // longjmp(env, val);
}
