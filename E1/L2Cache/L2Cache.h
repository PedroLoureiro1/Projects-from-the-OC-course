#ifndef L2CACHE_H
#define L2CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Cache.h"

void resetTime();

uint32_t getTime();

/****************  RAM memory (byte addressable) ***************/

void accessDRAM(uint32_t, uint8_t *, uint32_t);

/*********************** CacheLine *************************/

typedef struct CacheLine {
  uint8_t Data[BLOCK_SIZE];
  uint8_t Valid;
  uint8_t Dirty;
  uint32_t Tag;
} CacheLine;

/*********************** Cache L1 *************************/

#define L1_NLINES (L1_SIZE / BLOCK_SIZE)

void initCacheL1();

void accessL1(uint32_t, uint8_t *, uint32_t);

typedef struct L1 {
  uint32_t init;
  CacheLine lines[L1_NLINES];
} L1;

/*********************** Cache L2 *************************/
#define L2_NLINES (L2_SIZE / BLOCK_SIZE)

void initCacheL2();

void accessL2(uint32_t, uint8_t *, uint32_t);

typedef struct L2 {
  uint32_t init;
  CacheLine lines[L2_NLINES];
} L2;

/*********************** MainCache *************************/

typedef struct Cache {
  L1 l1;
  L2 l2;
} Cache;

void initCache();

/*********************** Interfaces *************************/

void read(uint32_t, uint8_t *);

void write(uint32_t, uint8_t *);


#endif
