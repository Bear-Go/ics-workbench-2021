#include <string.h>
#include "common.h"

static uint8_t mem[MEM_SIZE];
static uint8_t mem_diff[MEM_SIZE];

void init_mem(void) {
  int i;
  for (i = 0; i < MEM_SIZE; i ++) {
    mem[i] = rand() & 0xff;
  }
  
  memcpy(mem_diff, mem, MEM_SIZE);
}

// 从块号为`block_num`的内存地址中读出一整个cache块大小的内容到`buf`中
void mem_read(uintptr_t block_num, uint8_t *buf) {
  memcpy(buf, mem + (block_num << BLOCK_WIDTH), BLOCK_SIZE);
  cycle_increase(25);
}

// 往块号为`block_num`的内存地址中写入一整个cache块大小的内容`buf`
void mem_write(uintptr_t block_num, const uint8_t *buf) {
  memcpy(mem + (block_num << BLOCK_WIDTH), buf, BLOCK_SIZE);
  cycle_increase(6);
}

uint32_t mem_uncache_read(uintptr_t addr) {
  uint32_t *p = (void *)mem_diff + (addr & ~0x3);
  return *p;
}

void mem_uncache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  uint32_t *p = (void *)mem_diff + (addr & ~0x3);
  *p = (*p & ~wmask) | (data & wmask);
}
