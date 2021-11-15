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
  int cnt = 0;
  asm(
    "movl $0, %%eax\n\t" // ? local: rm wrong/oj: rm right 
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
  void *ret = NULL;
  asm(
    "movq %[dest],  %[ret]\n\t" // faster ?
    "rep movsb\n\t"
    : [dest] "+D"(dest), [ret] "+g"(ret)
    : "c"(n), "S"(src)
    : "cc", "memory"
  );
  return ret;
}

int asm_setjmp(asm_jmp_buf env) {
  asm(
    "mov  %[env], %%rax\n\t"
    "mov  %%rbx, (%%rax)\n\t"
    "mov  %%rsi, 8(%%rax)\n\t"
    "mov  %%rdi, 16(%%rax)\n\t"
    "mov  %%rbp, 24(%%rax)\n\t"
    "lea  8(%%rsp), %%rcx\n\t"
    "mov  %%rcx, 32(%%rax)\n\t"
    "mov  (%%rsp), %%rcx\n\t"
    "mov  %%rcx, 40(%%rax)\n\t"
    :
    : [env] "m"(env)
    : "%rax", "%rcx", "cc", "memory"
  );
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  asm(
    "mov  %[env], %%rdx\n\t"
    "mov  %[val], %%eax\n\t"
    "mov  (%%rdx),  %%rbx\n\t"
    "mov  8(%%rdx), %%rsi\n\t"
    "mov  16(%%rdx),  %%rdi\n\t"
    "mov  24(%%rdx),  %%rbp\n\t"
    "mov  32(%%rdx),  %%rsp\n\t"
    "jmp  *40(%%rdx)\n\t"
    :
    : [env] "m"(env), [val] "m"(val)
    : "%rax", "cc", "memory"
  );
}
