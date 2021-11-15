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
  void *ret = NULL;
  asm(
    "movq %[dest],  %[ret]\n\t"
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
    "mov  %[env], %%rcx\n\t"
    "mov  (%%rcx),  %%rbx\n\t"
    "mov  8(%%rcx), %%rsi\n\t"
    "mov  16(%%rcx),  %%rdi\n\t"
    "mov  24(%%rcx),  %%rbp\n\t"
    "mov  32(%%rcx),  %%rsp\n\t"
    "mov  %[val], %%rax\n\t"
    "jmp  *40(%%rcx)\n\t"
    :
    : [env] ""(env), [val] ""(val)
    : "%rax", "cc", "memory"
  );
  // longjmp(env, val);
}
