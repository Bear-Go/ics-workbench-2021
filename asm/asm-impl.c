#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
  asm(
    "addq %%rbx, %%rax\n\t"
    : "+a"(a)
    : "b"(b)
    );
  return a;
}

int asm_popcnt(uint64_t x) {
  int cnt;
  asm(
    "movl $0, %%eax\n\t"
    "movl $0, %%ecx\n\t"
    "inside:\n\t"
    "cmpl $64,  %%ecx\n\t"
    "jge  outside\n\t"
    "movq %%rbx,  %%rdx\n\t"
    "and  $1, %%rdx\n\t"
    "addl %%edx,  %%eax\n\t"
    "incl %%ecx\n\t"
    "shrq $1, %%rbx\n\t"
    "jmp  inside\n\t"
    "outside:"
    : "=a"(cnt)
    : "b"(x)
  );
  return cnt;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  void *ret = dest;
  asm volatile(
    "mov %[dest],  %[ret]\n\t"
    "rep movsb\n\t"
    : [dest] "+D"(dest), [ret] "+g"(ret)
    : "c"(n), "S"(src)
    : "cc", "memory"
  );
  return ret;
}

int asm_setjmp(asm_jmp_buf env) {
  return 0;
  // return setjmp(env);
}

void asm_longjmp(asm_jmp_buf env, int val) {
  // longjmp(env, val);
}
