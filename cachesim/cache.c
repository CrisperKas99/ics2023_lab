#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

typedef struct  
{
  bool valid;
  bool dirty;
  uint32_t tag;
  uint8_t data[BLOCK_SIZE];
} CacheLine;
 
typedef struct 
{
 CacheLine * lines;
} CacheSet;

typedef struct
{
  int8_t setindex;
  int8_t lineindex;
} Index;

static uint8_t SET_WIDTH;
static uint8_t ASSOCIATIVITY_WIDTH;
static uint32_t SET_SIZE;
static int SET_NUMBER;
static CacheSet * cache;

// addr: tag | cache set idx | block_offset
#define block_offset(addr) ((addr) & (BLOCK_SIZE - 1))
#define get_idx(addr) (((addr) >> BLOCK_WIDTH) & (SET_NUMBER - 1))
#define get_tag(addr) ((addr) >> (BLOCK_WIDTH + SET_WIDTH))

static inline uint32_t randomly_choose(uint32_t n) { return rand() % n; }

Index cache_access(uintptr_t addr){
  uint32_t idx = get_idx(addr);
  uint32_t tag = get_tag(addr);
  //printf("addr:%ld\n", addr);
  //printf("tag: %d,idx: %d\n", tag, idx);

  // whether cache hit
  CacheSet set = cache[idx];
  CacheLine * lines = set.lines;
  bool hit = false;
  int i ;
  for (i = 0; i < SET_SIZE; i++){
    if (lines[i].valid && lines[i].tag == tag){
      hit = true;
      break;
    }
  }
  // miss
  if ( !hit ){
    //printf("miss\n");
    uintptr_t block_num;
    i = randomly_choose(SET_SIZE);
    //write_back
    if (lines[i].valid && lines[i].dirty)
    {
      block_num = (lines[i].tag << SET_WIDTH) | idx;
      mem_write(block_num, lines[i].data);
    }
    block_num = addr >> BLOCK_WIDTH;
    mem_read(block_num, lines[i].data);
    lines[i].valid = true;
    lines[i].dirty = false;
    lines[i].tag = tag;
  }
  Index index;
  index.setindex = idx;
  index.lineindex = i;
  return index; 
}

uint32_t cache_read(uintptr_t addr) {
  Index index = cache_access(addr);
  
  uint8_t *p = (void *)cache[index.setindex].lines[index.lineindex].data + block_offset(addr & ~0x3);
  uint32_t *q = (uint32_t *)p;

  //printf("cache read: %d",*q);

  return *q;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  Index index = cache_access(addr);

  cache[index.setindex].lines[index.lineindex].dirty = true;

  uint8_t *p = (void *)cache[index.setindex].lines[index.lineindex].data + block_offset(addr & ~0x3);
  uint32_t *q = (uint32_t *)p;
  
  *q = (*q & ~wmask) | (data & wmask);
}

void init_cache(int total_size_width, int associativity_width) {
  SET_WIDTH = total_size_width - associativity_width - BLOCK_WIDTH; //6
  ASSOCIATIVITY_WIDTH = associativity_width; //2
  assert(SET_WIDTH == 6);
  //init cache
  SET_NUMBER = exp2(SET_WIDTH); //2^6

  SET_SIZE = exp2(associativity_width); //4
  cache = (CacheSet *) malloc(sizeof(CacheSet) * SET_NUMBER);
  for (int i=0; i < SET_NUMBER; i++){ //for every set
    cache[i].lines = (CacheLine *) malloc(sizeof(CacheLine) * SET_SIZE); //?
    CacheLine * lines = cache[i].lines;
    for(int j = 0; j < SET_SIZE; j++){
      lines[j].valid = false;
      lines[j].dirty = false;
    }
  }
}

void display_statistic(void) {
}
