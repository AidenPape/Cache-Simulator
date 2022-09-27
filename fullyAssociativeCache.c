#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>



struct cacheLine
{
  uint64_t fullMemAdd;
  struct cacheLine *prev;
  struct cacheLine *next;
};

struct cache
{
  struct cacheLine cacheArr[512];
  struct cacheLine *leastRecUsed;
  struct cacheLine *mostRecUsed;
  int inUse;
};

uint64_t asciiToBinStr(char ascii);
struct cacheLine* fulAssocCache(uint64_t memAdd, struct cache cacheSim);



int main(int argc, char *argv[])
{

  // FILE *fp;
  // fp = fopen("large","r");
  int totalMemAdd = 0;
  int cacheMisses = 0;
  int cacheHits = 0;


  char memAdd[16];
  struct cache cacheSim;
  for (int i=0; i<512; i++)
  {
    struct cacheLine null;
    null.fullMemAdd = 0;
    cacheSim.cacheArr[i]= null;
  }
  cacheSim.inUse=0;


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


    struct cacheLine* point;
    uint64_t tag = rawBinAdd>>6;
    // printf("tag = %p\n", tag);

    point = fulAssocCache(tag, cacheSim);
    if((*point).fullMemAdd==0){        //cache miss

      if(cacheSim.inUse==0){ // if first entry into cache
        cacheSim.cacheArr[0].fullMemAdd = tag; // store in first spot of array
        cacheSim.leastRecUsed = &cacheSim.cacheArr[0]; //and leastRecUsed
        // printf("first one\n");
      }
      else if(cacheSim.inUse==1){ //second entry into cache
        cacheSim.cacheArr[1].fullMemAdd = tag;
        cacheSim.mostRecUsed = &cacheSim.cacheArr[1];
        cacheSim.cacheArr[1].prev = &cacheSim.cacheArr[0];
        cacheSim.cacheArr[0].next = &cacheSim.cacheArr[1];
        // printf("Second miss\n");
      }

      else if(cacheSim.inUse<512){ //cache not full
        cacheSim.cacheArr[cacheSim.inUse].fullMemAdd = tag; // add cacheLine to next available spot
        cacheSim.cacheArr[cacheSim.inUse].prev = cacheSim.mostRecUsed; // assigned new cacheLine's prev to the previous mostRecUsed
        (*cacheSim.mostRecUsed).next = &(cacheSim.cacheArr[cacheSim.inUse]); //assign previous mostRecUsed next to new cacheLine
        cacheSim.mostRecUsed = &cacheSim.cacheArr[cacheSim.inUse]; // assign new cacheLine to mostRecUsed
        // printf("cache not full yet\n");
        // printf("Inuse = %d\n", cacheSim.inUse);
      }

      else{   //cache is full
        struct cacheLine* temp = cacheSim.leastRecUsed;  // save arr location of cacheLine that will be evicted
        cacheSim.leastRecUsed = (*cacheSim.leastRecUsed).next; //previous leastRecUsed's next is new leastRecUsed
        (*temp).fullMemAdd = tag; //rewrite new cacheLine over previously leastRecUsed
        (*cacheSim.mostRecUsed).next = temp; // link new node
        cacheSim.mostRecUsed = temp; //assign new cacheLine to mostRecUsed
      }
      cacheMisses++;
      if(cacheSim.inUse<512){
        cacheSim.inUse++;
      }
    }


    else{  //cache hit


      if(cacheSim.inUse<=1){
        //do nothing
      }

      else if((*point).fullMemAdd == (*cacheSim.leastRecUsed).fullMemAdd){ //if leastRecUsed is hit
        struct cacheLine* temp = cacheSim.leastRecUsed; //store real quick
        cacheSim.leastRecUsed = (*temp).next;
        (*temp).prev = cacheSim.mostRecUsed;
        (*cacheSim.mostRecUsed).next = temp;
        cacheSim.mostRecUsed = temp;
      }
      else{
        (*(*point).prev).next = (*point).next; //reassigning prev and next for cache hit
        (*cacheSim.mostRecUsed).next = point;
        cacheSim.mostRecUsed = point;
      }
      cacheHits++;
      // printf("Cache Hit\n");
      // printf("(4)data at leastRecUsed = %p\n", (*cacheSim.leastRecUsed).fullMemAdd);
    }



    totalMemAdd++;


    // for(int i=0;i<512;i++){
      // printf("Array data at index: %d %p\n", i, cacheSim.cacheArr[i].fullMemAdd);
    // }
    // printf("mostRecUsed %p\n", (*cacheSim.mostRecUsed).fullMemAdd);
    // printf("leastRecUsed.next: %p\n", (*(*cacheSim.leastRecUsed).next).fullMemAdd);
    // printf("mostRecUsed %p\n", (*cacheSim.mostRecUsed).fullMemAdd);
    // printf("mostRecUsed.prev: %p\n", (*(*cacheSim.mostRecUsed).prev).fullMemAdd);
    // printf("Total Addresses Cached: %d\n", totalMemAdd);
    // printf("Total Memory Addresses %d\n", totalMemAdd);
    // printf("Total Cache Misses %d\n", cacheMisses);
    // printf("Total Cache Hits %d\n", cacheHits);

  }
  //
  printf("Total Memory Addresses %d\n", totalMemAdd);
  printf("Total Cache Misses %d\n", cacheMisses);
  printf("Total Cache Hits %d\n", cacheHits);

  //printf("%d\n", totalMemAdd);

  return 0;

}


struct cacheLine* fulAssocCache(uint64_t memAdd, struct cache cacheSim)
{

  struct cacheLine signal;
  signal.fullMemAdd = 0;
  struct cacheLine* pointer;

  for(int i=0; i<512; i++){
    if(cacheSim.cacheArr[i].fullMemAdd==memAdd){
      pointer = &cacheSim.cacheArr[i];
      return pointer;
    }
  }

  pointer = &signal;
  return pointer;
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
