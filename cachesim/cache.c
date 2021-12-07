#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

typedef struct {
  bool valid_bit;
  bool dirty_bit;
  uint32_t tag;
  uint8_t data[BLOCK_SIZE];
} line;

static line *cache;

static uint32_t LINE_NUM = 0; // the number of lines in cache
static uint32_t SET_SIZE = 0; // how many lines in a set/group
static uint32_t SET_NUM = 0; // how many sets/groups in cache
static uint32_t INDEX_WIDTH = 0; // how many bits of index of set/group

#define INDEX(addr) ((addr >> BLOCK_WIDTH) & mask_with_len(INDEX_WIDTH))
#define TAG(addr) ((addr >> (BLOCK_WIDTH + INDEX_WIDTH)) & mask_with_len(MEM_WIDTH - BLOCK_WIDTH - INDEX_WIDTH))
#define ADDR_IN_BLOCK(addr) (addr & mask_with_len(BLOCK_WIDTH))
#define BLOCK_NUM(addr) ((addr >> BLOCK_WIDTH) & mask_with_len(MEM_WIDTH - BLOCK_WIDTH))
#define MEM_WIDTH 25

// 从 cache 中读出 addr 地址处的 4 字节数据
// 若缺失，需要先从内存中读入数据
uint32_t cache_read(uintptr_t addr) {

  // decomp addr
  addr &= mask_with_len(MEM_WIDTH);
  uint32_t index = INDEX(addr); // index of set
  uint32_t tag = TAG(addr); // tag of block
  uint32_t addr_in_block = ADDR_IN_BLOCK(addr); // addr in block
  line *this_cache = &cache[SET_SIZE * index];

  printf("addr : 0x%08lx\n", addr);
  printf("index of set : %d\n", index);
  printf("tag : 0x%08x\n", tag);
  printf("addr in block : 0x%08x\n", addr_in_block);

  // check whether hit
  for (int i = 0; i < SET_SIZE; ++ i) {
    // hit
    cycle_increase(1);
    if ((this_cache[i].tag == tag) && this_cache[i].valid_bit == true) {
      uint32_t *ret = (uint32_t *)(this_cache[i].data + addr_in_block);
      printf("hit!\n");
      return *ret;
    }
  }

  // miss缺失
  uint32_t block_num = BLOCK_NUM(addr);

  // exist invalid bit
  this_cache = &cache[SET_SIZE * index];
  for (int i = 0; i < SET_SIZE; ++ i) {
    cycle_increase(1);
    if (this_cache[i].valid_bit == false) {
      // read the block into this line
      mem_read(block_num, this_cache[i].data);
      this_cache[i].valid_bit = true;
      this_cache[i].dirty_bit = false;
      this_cache[i].tag = TAG(addr);
      uint32_t *ret = (uint32_t *)(this_cache[i].data + addr_in_block); // duo du le liang ge zi jie
      uint8_t *t = (uint8_t *)ret;
      ret = (uint32_t *)(t-1);
      printf("0x%02x\n", *t);
      printf("0x%02x\n", *(t-2));
      printf("exist invalid bit\n");
      printf("cache read : 0x%08x\n", *ret);
      return *ret;
    }
  }

  // all valid bits
  int idx = rand() % SET_SIZE;
  this_cache = &cache[SET_SIZE * index];
  if (this_cache[idx].dirty_bit == true) {
    mem_write((this_cache[idx].tag << INDEX_WIDTH) | index, this_cache[idx].data);
  }
  // read the block into this line
  mem_read(block_num, this_cache[idx].data);
  this_cache[idx].valid_bit = true;
  this_cache[idx].dirty_bit = false;
  this_cache[idx].tag = TAG(addr);
  uint32_t *ret = (uint32_t *)(this_cache[idx].data + addr_in_block);
  printf("all valid bits\n");
  return *ret;

}
  
// 往 cache 中 addr 地址所属的块写入数据 data，写掩码为 wmask
// 例如当 wmask 为 0xff 时，只写入低8比特
// 若缺失，需要从先内存中读入数据
void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  
  // decomp addr
  addr &= mask_with_len(MEM_WIDTH);
  uint32_t index = INDEX(addr); // index of set
  uint32_t tag = TAG(addr); // tag of block
  uint32_t addr_in_block = ADDR_IN_BLOCK(addr); // addr in block
  line *this_cache = &cache[SET_SIZE * index];

  // check every line of this set
  for (int i = 0; i < SET_SIZE; ++ i) {
    cycle_increase(1);
    // hit
    if ((this_cache[i].tag == tag) && this_cache[i].valid_bit == true) {
      uint32_t *ret = (uint32_t *)(this_cache[i].data + addr_in_block);
      *ret = (*ret & ~wmask) | (data & wmask);
      this_cache[i].dirty_bit = true;
      return;
    }
  }
  // miss缺失
  uint32_t block_num = BLOCK_NUM(addr);

  // exist invalid bit
  for (int i = 0; i < SET_SIZE; ++ i) {
    cycle_increase(1);
    if (this_cache[i].valid_bit == false) {
      // read the block into this line
      mem_read(block_num, this_cache[i].data);
      this_cache[i].valid_bit = true;
      this_cache[i].dirty_bit = false;
      this_cache[i].tag = TAG(addr);
      uint32_t *ret = (uint32_t *)(this_cache[i].data + addr_in_block);
      *ret = (*ret & ~wmask) | (data & wmask);
      this_cache[i].dirty_bit = true;
      return;
    }
  }

  // all valid bits
  int idx = rand() % SET_SIZE;
  if (this_cache[idx].dirty_bit == true) {
    mem_write((this_cache[idx].tag << INDEX_WIDTH) | index, this_cache[idx].data);
  }
  // read the block into this line
  mem_read(block_num, this_cache[idx].data);
  this_cache[idx].valid_bit = true;
  this_cache[idx].dirty_bit = false;
  this_cache[idx].tag = TAG(addr);
  uint32_t *ret = (uint32_t *)(this_cache[idx].data + addr_in_block);
  *ret = (*ret & ~wmask) | (data & wmask);
  this_cache[idx].dirty_bit = true;
  return;

}

// 初始化一个数据大小为 2^total_size_width B，关联度为 2^associativity_width 的 cache
// 例如 init_cache(14, 2) 将初始化一个 16KB，4 路组相联的cache
// 将所有 valid bit 置为无效即可
void init_cache(int total_size_width, int associativity_width) {

  LINE_NUM = exp2(total_size_width) / BLOCK_SIZE;
  SET_SIZE = exp2(associativity_width);
  SET_NUM = LINE_NUM / SET_SIZE;
  INDEX_WIDTH = total_size_width - BLOCK_WIDTH - associativity_width;
  
  // create the cache
  cache = malloc(sizeof(line) * LINE_NUM);
  // set valid bits & dirty bits
  for (int i = 0; i < LINE_NUM; ++ i) {
    cache[i].valid_bit = false;
    cache[i].dirty_bit = false;
  }

}

void display_statistic(void) {
}
