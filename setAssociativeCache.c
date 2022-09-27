#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>



struct cacheLine
{
  uint64_t fullMemAdd;
  int prev;
  int next;
  int arrLoc;
};

struct cache
{
  struct cacheLine cacheArr[8];
  int LRU;
  int MRU;
  int inUse;
};

uint64_t asciiToBinStr(char ascii);
int setAssocCache(uint64_t memAdd, struct cache cache);



int main(int argc, char *argv[])
{

  // FILE *fp;
  // fp = fopen("large","r");
  int totalMemAdd = 0;
  int cacheMisses = 0;
  int cacheHits = 0;

  char memAdd[16];

  struct cache fullCache[64];

  for(int i = 0; i<64; i++)
  {
    fullCache[i].inUse = 0;
    fullCache[i].MRU = 0;
    fullCache[i].LRU = 0;
    for(int j=0; j>8; j++)
    {
      struct cacheLine null;
      null.fullMemAdd = 0;
      fullCache[i].cacheArr[j]=null;
    }
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



    uint64_t set = rawBinAdd>>6 & 0x3f;
    uint64_t tag = rawBinAdd>>12;

    int setIndex = set;
    // printf("tag = %d\n", setIndex);

    for(int i = 0; i<64; i++)
    {
      // printf("array , %d", i);
      // printf(": inuse= %d\n",fullCache[i].inUse);
    }


    int point;
    // printf("tag to cache %p\n", tag);
    point = setAssocCache(tag, fullCache[setIndex]);
    // printf("array number %d\n", setIndex);
    // uint64_t tester = (*point).fullMemAdd;
    // printf("inUse = %d\n", fullCache[setIndex].inUse);
    // printf("Pointer returned: %d\n", point);
    int test = point;

    // printf("inUse for array: %d\n", fullCache[setIndex].inUse);



    if(test == -1){        //cache miss
      // printf("in cache miss loop\n");
      if(fullCache[setIndex].inUse==0){ // if first entry into cache
        fullCache[setIndex].cacheArr[0].arrLoc = 0;
        fullCache[setIndex].cacheArr[0].fullMemAdd = tag; // store in first spot of array
        fullCache[setIndex].LRU = 0; //and cacheArr[LRU]
        // printf("first one\n");


      }
      else if(fullCache[setIndex].inUse==1){ //second entry into cache
        fullCache[setIndex].cacheArr[1].fullMemAdd = tag;
        fullCache[setIndex].cacheArr[1].arrLoc = 1;
        fullCache[setIndex].MRU = 1;
        fullCache[setIndex].cacheArr[1].prev = 0;
        fullCache[setIndex].cacheArr[0].next = 1;
        // printf("Second miss\n");
      }

      else if(fullCache[setIndex].inUse<8){ //cache not full
        fullCache[setIndex].cacheArr[fullCache[setIndex].inUse].fullMemAdd = tag; // add cacheLine to next available spot
        fullCache[setIndex].cacheArr[fullCache[setIndex].inUse].arrLoc=fullCache[setIndex].inUse;
        fullCache[setIndex].cacheArr[fullCache[setIndex].inUse].prev = fullCache[setIndex].MRU; // assigned new cacheLine's prev to the previous cacheArr[MRU]
        fullCache[setIndex].cacheArr[fullCache[setIndex].MRU].next = fullCache[setIndex].inUse; //assign previous cacheArr[MRU] next to new cacheLine
        fullCache[setIndex].MRU = fullCache[setIndex].inUse; // assign new cacheLine to cacheArr[MRU]
        // printf("cache not full yet\n");
        // printf("Inuse = %d\n", fullCache[setIndex].inUse);
      }

      else{   //cache is full
        // printf("cacheArr[LRU] is %p\n",(*fullCache[setIndex].cacheArr[LRU]).fullMemAdd);
        int temp = fullCache[setIndex].LRU;  // save arr location of cacheLine that will be evicted
        fullCache[setIndex].cacheArr[fullCache[setIndex].LRU] = fullCache[setIndex].cacheArr[fullCache[setIndex].cacheArr[fullCache[setIndex].LRU].next]; //previous cacheArr[LRU]'s next is new cacheArr[LRU]
        fullCache[setIndex].cacheArr[temp].fullMemAdd = tag; //rewrite new cacheLine over previously cacheArr[LRU]
        fullCache[setIndex].cacheArr[temp].arrLoc = fullCache[setIndex].LRU;
        fullCache[setIndex].cacheArr[fullCache[setIndex].MRU].next = temp; // link new node
        fullCache[setIndex].MRU = fullCache[setIndex].cacheArr[temp].arrLoc; //assign new cacheLine to cacheArr[MRU]
      }
      cacheMisses++;

      if(fullCache[setIndex].inUse<8){
        fullCache[setIndex].inUse++;
      }
    }

    else{  //cache hit
      // printf("cache hit\n");
      // printf("pointer = %p\n", fullCache[setIndex].cacheArr[point].fullMemAdd);
      // printf("LRU %d\n", fullCache[setIndex].LRU);
      // printf("next arrloc %d\n", (*(fullCache[setIndex].cacheArr[fullCache[setIndex].LRU].next)).arrLoc);
      if(fullCache[setIndex].inUse==1){
        //do nothing
      }

      else if(fullCache[setIndex].cacheArr[point].fullMemAdd == fullCache[setIndex].cacheArr[fullCache[setIndex].LRU].fullMemAdd){ //if cacheArr[LRU] is hit
        int temp = fullCache[setIndex].LRU; //store real quick
        fullCache[setIndex].LRU = fullCache[setIndex].cacheArr[temp].arrLoc;
        fullCache[setIndex].cacheArr[temp].prev = fullCache[setIndex].MRU;
        fullCache[setIndex].cacheArr[fullCache[setIndex].MRU].next = temp;
        fullCache[setIndex].cacheArr[fullCache[setIndex].MRU] = fullCache[setIndex].cacheArr[temp];
      }
      else{
        fullCache[setIndex].cacheArr[fullCache[setIndex].cacheArr[point].prev].next = fullCache[setIndex].cacheArr[point].next; //reassigning prev and next for cache hit
        fullCache[setIndex].cacheArr[fullCache[setIndex].MRU].next = point;
        fullCache[setIndex].MRU = point;
      }
      cacheHits++;
      // printf("in miss loop, cacheArr[LRU]: %p\n", (*(fullCache[setIndex].cacheArr[LRU])).fullMemAdd);
      // printf("Cache Hit\n");
      // printf("(4)data at cacheArr[LRU] = %p\n", (*fullCache[setIndex].cacheArr[LRU]).fullMemAdd);
    }

    // printf("LRU = %d\n", fullCache[setIndex].LRU);

    totalMemAdd++;

  }

  printf("Total Memory Addresses %d\n", totalMemAdd);
  printf("Total Cache Misses %d\n", cacheMisses);
  printf("Total Cache Hits %d\n", cacheHits);

  //printf("%d\n", totalMemAdd);

  return 0;

}



int setAssocCache(uint64_t memAdd, struct cache cache)
{

  // printf("memAdd = %p\n", memAdd);

  int index;
  // printf("(in func: memAdd being checked = %p\n", memAdd);

  for(int i=0; i<8; i++){
    // printf("elements of cache %p\n", cache.cacheArr[i].fullMemAdd);
    if(cache.cacheArr[i].fullMemAdd==memAdd){
      index = i;
      // printf("match\n");
      // printf("pointer points to %p\n", (*pointer).fullMemAdd);
      return index;
    }
  }

  index = -1;
  return index;;
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
