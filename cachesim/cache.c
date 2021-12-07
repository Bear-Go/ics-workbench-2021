#include "common.h"
#include <inttypes.h>
// #include <string.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

typedef struct {
  bool valid_bit;
  uint32_t tag;
  uint8_t data[BLOCK_SIZE];
} line;

static line *cache;

static uint32_t LINE_NUM = 0;
static uint32_t SET_SIZE = 0;
static uint32_t SET_NUM = 0;
static uint32_t INDEX_WIDTH = 0;

#define index_of_addr(addr) ((addr >> BLOCK_WIDTH) & mask_with_len(INDEX_WIDTH))

// 从 cache 中读出 addr 地址处的 4 字节数据
// 若缺失，需要先从内存中读入数据
uint32_t cache_read(uintptr_t addr) {
  // get the block_num from addr
  uint32_t index = (addr >> BLOCK_WIDTH) & mask_with_len(INDEX_WIDTH);

  printf("index %d\n", index);
  return 0;
}

// 往 cache 中 addr 地址所属的块写入数据 data，写掩码为 wmask
// 例如当 wmask 为 0xff 时，只写入低8比特
// 若缺失，需要从先内存中读入数据
void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
}

// 初始化一个数据大小为 2^total_size_width B，关联度为 2^associativity_width 的 cache
// 例如 init_cache(14, 2) 将初始化一个 16KB，4 路组相联的cache
// 将所有 valid bit 置为无效即可
void init_cache(int total_size_width, int associativity_width) {
  LINE_NUM = exp2(total_size_width) / BLOCK_SIZE;
  SET_SIZE = exp2(associativity_width);
  SET_NUM = LINE_NUM / SET_SIZE;
  INDEX_WIDTH = total_size_width - BLOCK_WIDTH - associativity_width;
  printf("line num = %d\n", LINE_NUM);
  printf("set size = %d\n", SET_SIZE);
  printf("set num = %d\n", SET_NUM);
  printf("index width = %d\n", INDEX_WIDTH);
  // create the cache
  cache = (line *)malloc(sizeof(line) * LINE_NUM);
  // set valid bits
  for (int i = 0; i < LINE_NUM; ++ i) cache[i].valid_bit = 0;
}

void display_statistic(void) {
}
