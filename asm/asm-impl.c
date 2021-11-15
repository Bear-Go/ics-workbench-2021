#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
  asm(
    "addq %%rbx, %%rax\n\t" // simple
    : "+a"(a)
    : "b"(b)
    );
  return a;
}
 
int asm_popcnt(uint64_t x) {
  int cnt = 0;
  asm( 
    /*  
    * Obeserve: gcc may do something to make up
    * for the missing part between asm & c language
    * Example: 
    * When using a function it seems that both writing
    * and not writing the push part in asm are correct.
    * so what happens during this procedure 
    * maybe a clearer asm coding style is needed
    */
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
    "movq %[dest],  %[ret]\n\t" // hard version
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
    "mov  %%rbx, (%%rax)\n\t" // save the env %rbc %rsi %rdi %rbp %rsp+8 %rsp
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
  /*
  * When trying to get the meaning of setjmp & longjmp
  * I find that these two function cannot be packaged into
  * another two functions after RTFW I got a possible ans
  * that is the package will disrupt the order of the exc
  * and this leads to the error of longjmp
  */
  asm( // naive version : no signal mask & not check val ?= 0 
    "mov  %[env], %%rdx\n\t"  // backto the env & change the %eax to val
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
