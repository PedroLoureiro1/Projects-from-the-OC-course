#include "L2WCache.h"

uint8_t L1Cache[L1_SIZE];
uint8_t L2Cache[L2_SIZE];
uint8_t DRAM[DRAM_SIZE];
uint32_t time;
Cache SimpleCache;

  /**************** Address extraction **************/

  //mask the address to get the offset
  uint32_t getOffset(uint32_t address) { return address & 0x3F; }

  //mask the address to get the index
  uint32_t getIndex(uint32_t address) { return (address >> 6) & 0xFF; }

  //mask the address to get the tag
  uint32_t getTag(uint32_t address) { return address >> 14 ; }


/**************** Time Manipulation ***************/

void resetTime() { time = 0; }

uint32_t getTime() { return time; }


/****************  RAM memory (byte addressable) ***************/

void accessDRAM(uint32_t address, uint8_t *data, uint32_t mode) {

  if (address >= DRAM_SIZE - WORD_SIZE + 1)
    exit(-1);

  if (mode == MODE_READ) {
    memcpy(data, &(DRAM[address]), BLOCK_SIZE);
    time += DRAM_READ_TIME;
  }

  if (mode == MODE_WRITE) {
    memcpy(&(DRAM[address]), data, BLOCK_SIZE);
    time += DRAM_WRITE_TIME;
  }
}

/*********************** init *************************/

/* Initialize L1 */
void initL1(){

/* go through each line and set all properties to 0 */
  for (int i = 0; i < L1_NLINES; i++) {
        SimpleCache.l1.lines[i].Valid = 0;
        SimpleCache.l1.lines[i].Dirty = 0;
        SimpleCache.l1.lines[i].Tag = 0;
  
        /* set all words to 0 */
        for (int j = 0; j < BLOCK_SIZE; j+= WORD_SIZE) {
          SimpleCache.l1.lines[i].Data[j] = 0;
        }
      }
  
      SimpleCache.l1.init = 1;
}


/* Initialize L2 */
void initL2() {
  
    /* go through each line and set all properties to 0 */
    for (int i = 0; i < L2_NLINES; i++) {
      /* go through each way and set all properties to 0 */
      for(int j = 0; j < TWO_WAY; j++) {
        SimpleCache.l2.sets[i].lines[j].Valid = 0;
        SimpleCache.l2.sets[i].lines[j].Dirty = 0;
        SimpleCache.l2.sets[i].lines[j].Tag = 0;

        /* set all words to 0 */
        for (int k = 0; k < BLOCK_SIZE; k+=WORD_SIZE) {
          SimpleCache.l2.sets[i].lines[j].Data[k] = 0;
        }
      }
      SimpleCache.l2.sets[i].LRU = 0;  // Initialize LRU bit to 0
    }

    SimpleCache.l2.init = 1;
}

void initCache() { 
  SimpleCache.l1.init = 0;
  SimpleCache.l2.init = 0;
  initL1();
  initL2();
}

/*********************** L1 cache *************************/

void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {

  uint32_t index, Tag, offset;
  uint8_t TempBlock[BLOCK_SIZE];


  index = getIndex(address);
  Tag = getTag(address);
  offset = getOffset(address);

  CacheLine *Line = &SimpleCache.l1.lines[index];
 
  // If HIT case:
  if (Line->Valid && Line->Tag == Tag) {

    if (mode == MODE_READ) {
      memcpy(data, &Line->Data[offset], WORD_SIZE);

      time += L1_READ_TIME;
    }

    if (mode == MODE_WRITE) {
      memcpy(&Line->Data[offset], data, WORD_SIZE);

      Line->Dirty = 1;
      time += L1_WRITE_TIME;
    }
  }

  // If MISS case:
  else {
    
    accessL2(address - offset, TempBlock, MODE_READ);
    // Copy dirty data to the next level cache
    if (Line->Valid && Line->Dirty) {
      uint32_t Tag_aux = Line->Tag << 14;
      uint32_t Index_aux = getIndex(address) << 6;
      uint32_t Adress_aux = Tag_aux | Index_aux;
      accessL2(Adress_aux, Line->Data, MODE_WRITE);
    }

    // Update cache with new data
    memcpy(&Line->Data, TempBlock,BLOCK_SIZE);
    Line->Valid = 1;
    Line->Dirty = 0;
    Line->Tag = Tag;

    if (mode == MODE_READ) {
      memcpy(data, &Line->Data[offset], WORD_SIZE);

      time += L1_READ_TIME;
    }

    if (mode == MODE_WRITE) {
      memcpy(&Line->Data[offset], data, WORD_SIZE);

      Line->Dirty = 1;
      time += L1_WRITE_TIME;
    }
  }
}

/*********************** L2 cache *************************/
void accessL2(uint32_t address, uint8_t *data, uint32_t mode) {

  uint32_t index, Tag, offset;
  uint8_t TempBlock[BLOCK_SIZE];


  index = getIndex(address);
  Tag = getTag(address);
  offset = getOffset(address);

  Set *set = &SimpleCache.l2.sets[index];
  
  for (int i = 0; i < TWO_WAY; i++) {
    CacheLine *Line = &SimpleCache.l2.sets[index].lines[i];
  
    // If HIT case:
    if (Line->Valid && Line->Tag == Tag) {

      if (mode == MODE_READ) {
        memcpy(data, &Line->Data[offset], BLOCK_SIZE);

        time += L2_READ_TIME;
      }

      if (mode == MODE_WRITE) {
        memcpy(&Line->Data[offset], data, BLOCK_SIZE);

        Line->Dirty = 1;
        time += L2_WRITE_TIME;
      }

      // Update LRU bit (i.e., mark this line as most recently used)
      set->LRU = (i == 0) ? 1 : 0;  // If way 0 was used, set LRU = 1, else LRU = 0

      return;
    }
  }
  
  //MISS case:
  
  CacheLine *victimLine = (set->LRU == 0) ? &SimpleCache.l2.sets[index].lines[0] : &SimpleCache.l2.sets[index].lines[1];

  accessDRAM(address - offset, TempBlock, MODE_READ);
  // Copy dirty data to Primary Storage 
  if (victimLine->Valid && victimLine->Dirty) {
      uint32_t Tag_aux = victimLine->Tag << 14;
      uint32_t Index_aux = getIndex(address) << 6;
      uint32_t Adress_aux = Tag_aux | Index_aux;
      accessDRAM(Adress_aux, victimLine->Data, MODE_WRITE);
  }
  // Update cache with new data

  memcpy(&victimLine->Data, TempBlock,BLOCK_SIZE);
  victimLine->Valid = 1;
  victimLine->Dirty = 0;
  victimLine->Tag = Tag;

  if (mode == MODE_READ) {
    memcpy(data, &victimLine->Data[offset], WORD_SIZE);
    time += L2_READ_TIME;
  }
  if (mode == MODE_WRITE) {
    memcpy(&victimLine->Data[offset], data, WORD_SIZE);
    victimLine->Dirty = 1;
    time += L2_WRITE_TIME;
  }

  set->LRU = (set->LRU == 0) ? 1 : 0;  // Update LRU bit (toggle)
}


void read(uint32_t address, uint8_t *data) {
  accessL1(address, data, MODE_READ);
}

void write(uint32_t address, uint8_t *data) {
  accessL1(address, data, MODE_WRITE);
}
