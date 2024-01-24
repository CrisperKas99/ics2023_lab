#include "asm.h"
#include <string.h>
#include <stdio.h>

//return signed 64 bit a+b
int64_t asm_add(int64_t a, int64_t b) {
  int64_t result=0;
  __asm__ volatile("movq %1, %0\n\t"
      "addq %2, %0\n"
      : "=g" (result)
      : "g" (a),"g"(b)
  );
  return result;
}

//return unsigned 1 times
int asm_popcnt(uint64_t x) {
  int64_t result = 0;
  __asm__ volatile("popcnt %1, %0;"
      : "=r" (result)
      : "r" (x)
    );
  return result;
}


void *asm_memcpy(void *dest, const void *src, size_t n) {
  __asm__ volatile("1:\tlodsb\n\t"
      "stosb\n\t"
      "subl $0x1, %%edx\n\t" 
      "testl %%edx,%%edx\n\t"
      "jne 1b"
      :
      :"r"(src), "r"(dest), "d"(n)
      :"memory"
    );
    return dest;
}

// rbx rbp rsp r12 r13 r14 r15 pc

int asm_setjmp(asm_jmp_buf env) {
  __asm__ volatile(
    "movq %%rbx, (%%rdi)\n\t"
    "movq %%rbp, 0x8(%%rdi)\n\t"
    "movq %%r12, 0x10(%%rdi)\n\t"
    "movq %%r13, 0x18(%%rdi)\n\t"  
    "movq %%r14, 0x20(%%rdi)\n\t"
    "movq %%r15, 0x28(%%rdi)\n\t"

    "lea 0x8(%%rsp), %%rcx\n\t"
    "movq %%rcx, 0x30(%%rdi)\n\t"

    "movq (%%rsp), %%rax \n\t"  
    "movq %%rax, 0x38(%%rdi)\n\t"
    :
    : "rdi"(env)
    : "memory"
  );
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
    __asm__ volatile(
    "movq (%%rdi)   ,  %%rbx \n\t"
    "movq 0x8(%%rdi),  %%rbp \n\t"
    "movq 0x10(%%rdi), %%r12 \n\t"
    "movq 0x18(%%rdi), %%r13 \n\t"  
    "movq 0x20(%%rdi), %%r14 \n\t"
    "movq 0x28(%%rdi), %%r15 \n\t"
    "movq 0x30(%%rdi), %%rsp \n\t"
    "movq 0x38(%%rdi), %%rcx \n\t"
    "movq %%rsi, %%rax \n\t"
    "jmp *%%rcx "
    :
    : "rdi"(env), "rsi"(val)
    : "memory", "rcx"
  );
}

