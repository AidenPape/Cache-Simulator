#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


struct cache
{
  uint64_t cacheArr[512];
};

uint64_t asciiToBinStr(char ascii);
bool dirMapCache(uint64_t memAdd, struct cache cacheSim, int arrayIndex);


int main(int argc, char *argv[])
{

  // FILE *fp;
  // fp = fopen("large","r");
  int totalMemAdd = 0;
  int cacheMisses = 0;
  int cacheHits = 0;
  int evictions = 0;

  char memAdd[16];
  struct cache cacheSim;
  for (int i=0; i<512; i++)
  {
    uint64_t null = 0;
    cacheSim.cacheArr[i]= null;
  }

  while(fgets(memAdd, 16, stdin) != NULL) //fix this to account for a memory addess that
  {                                     // comes in with some other amount of digits
    uint64_t rawBinAdd= 0x0000000000000000;
    // printf("Length of text line %d\n", strlen(memAdd));
    // printf("memAdd = %s\n", memAdd);

    if(strlen(memAdd)==10)
    {
      for (int i=0; i<9; i++)
      {
        uint64_t byte = asciiToBinStr(memAdd[i]);
        rawBinAdd = rawBinAdd | (byte>>((i+7)*4));
        // printf("(9)Inside binary %p\n", rawBinAdd);
      }
    }

    if(strlen(memAdd)==9)
    {
      for (int i=0; i<8; i++)
      {
        uint64_t byte = asciiToBinStr(memAdd[i]);
        rawBinAdd = rawBinAdd | (byte>>((i+8)*4));
        // printf("(8)Inside binary %p\n", rawBinAdd);
      }
    }
    // printf("Full binary %p\n", rawBinAdd);

    // printf("Memory Address: %p\n", rawBinAdd);

    bool hit;

    uint64_t enTag = rawBinAdd>>6 & 0x1ff;
    uint64_t tag = rawBinAdd>>6;
    int arrIndex = enTag;
    // printf("Array Index %d\n", arrIndex);


    hit = dirMapCache(tag, cacheSim, arrIndex);
    if(!hit){
      if(cacheSim.cacheArr[arrIndex]!= 0){
        evictions++;
      }
      cacheSim.cacheArr[arrIndex] = tag;
      cacheMisses++;
      // printf("Cache Miss\n");
    }
    else{
      cacheHits++;
      // printf("Cache Hit\n");
    }

    totalMemAdd++;
  }

  printf("Total Memory Addresses %d\n", totalMemAdd);
  printf("Total Cache Misses %d\n", cacheMisses);
  printf("Total Cache Hits %d\n", cacheHits);
  // printf("Total Evictions %d\n", evictions);

  return 0;

}


bool dirMapCache(uint64_t memTag, struct cache cacheSim, int arrayIndex)
{
  bool cacheHit;

  // printf("Memory address current in cache: %p\n", cacheSim.cacheArr[arrayIndex]);
  // printf("memtag = %p\n", memTag);

  if(cacheSim.cacheArr[arrayIndex]==memTag){
    // printf("Cache Hit\n");
    cacheHit = true;
    return cacheHit;
  }
  else{
    // printf("Cache Miss\n");
  }

  cacheHit = false;
  return cacheHit;
}



uint64_t asciiToBinStr(char ascii)
{
  uint64_t byte;

  if(ascii==48){
    byte = 0x0000000000000000;
  }

  if(ascii==49){
    byte = 0x1000000000000000;
  }

  if(ascii==50){
    byte = 0x2000000000000000;
  }

  if(ascii==51){
    byte = 0x3000000000000000;
  }

  if(ascii==52){
    byte = 0x4000000000000000;
  }

  if(ascii==53){
    byte = 0x5000000000000000;
  }

  if(ascii==54){
    byte = 0x6000000000000000;
  }

  if(ascii==55){
    byte = 0x7000000000000000;
  }

  if(ascii==56){
    byte = 0x8000000000000000;
  }

  if(ascii==57){
    byte = 0x9000000000000000;
  }

  if(ascii==97){
    byte = 0xa000000000000000;
  }

  if(ascii==98){
    byte = 0xb000000000000000;
  }

  if(ascii==99){
    byte = 0xc000000000000000;
  }

  if(ascii==100){
    byte = 0xd000000000000000;
  }

  if(ascii==101){
    byte = 0xe000000000000000;
  }

  if(ascii==102){
    byte = 0xf000000000000000;
  }

  return byte;
}
