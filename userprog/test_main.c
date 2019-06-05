#include <stdio.h>
#include <random.h>
#include "threads/malloc.h"
#include "devices/timer.h"
#include "threads/palloc.h"
#include "userprog/test_main.h"

#include "threads/loader.h"

int testpalloc_add100_remove50_add50_remove100(int seed);
int test_fragmentation(int seed);

void
palloc_test_main (void) 
{
  int testResult = 0;
  int i;
  //test_name = argv[0];

  printf ("begin testing\n");
  printf("\n");
  printf ("begin testing speed with seed 1~100\n");
  
  for( i =0;i<100;i++)
      testResult +=  testpalloc_add10_remove5_add5_remove10(1);//seeded test, to should have stable outcome
  printf ("ended speed test, elapsed time : %d.%04d seconds\n\n", testResult/1000, testResult%1000);
  
  printf("\n");
  printf ("begin testing fragmentation with seed 1~100\n");
  for( i =0;i<100;i++)
    testResult += test_fragmentation(i);
  printf ("ended fragmentation test, avg.score : %d.%02d\n\n", testResult/100, testResult%100);

  shutdown_power_off();
  return 0;

}

int testpalloc_add10_remove5_add5_remove10(int seed)
{
  int start_time = timer_ticks();
  void* segments[100];
  int segSize[100];
  int i,loop;

  random_init (seed);

  for(loop = 0;loop<10;loop++)
  {
    for( i =0;i<10;i++)//add 10 random
      segments[i] = palloc_get_multiple(PAL_USER,segSize[i] = random_ulong()%16+1);//get 1~16 random pagecd

    for(i = 0;i<10;i+=2)//remove 
      palloc_free_multiple(segments[i],segSize[i]);

    for(i = 0;i<10;i+=2)//add 5, random
      segments[i] = palloc_get_multiple(PAL_USER,segSize[i] = random_ulong()%16+1);//get 1~16 random page

    elapsed_time = timer_ticks() - start_time;//save timer status.

    for(i = 0;i<10;i++)
      palloc_free_multiple(segments[i],segSize[i]);
  }
  
  int elapsed_time;
    

  //todo: check segmentation here
  



  

  return elapsed_time;
}

//http://www.vldb.org/pvldb/vol11/p458-merritt.pdf, table 4: 

/*
                  Benchmark derived from prior work [59]:
  Fragmentation   Given a pair of sizes, allocate objects of first size,
    benchmark     randomly delete 90%, allocate objects of second
                  size until allocation fails (or until a specific
                  capacity has been reached).
  
  >change all part to seeded random.
*/
int test_fragmentation(int seed)
{
  int start_time;
  int elapsed_time;
  void** segments = malloc(init_ram_pages*sizeof(void*));
  int *segSize = malloc(init_ram_pages*sizeof(int));
  int segcnt, i, idx, addedCnt;

  printf("fragmentation bench started, (seed: %d), tot.page = %d\n", seed, init_ram_pages);

  start_time = timer_ticks();
  for( segcnt =0; 1 ;segcnt++)//add i random
  {
    segments[segcnt] = palloc_get_multiple(PAL_USER,segSize[segcnt] = random_ulong()%8+1);//get 1~8 random pagecd
    if(segments[segcnt] == NULL ) break;
  }
  segcnt++;
  for(i=0 ; i<segcnt ; i++)
  {
    if(i%10 == 0)
      continue;
    palloc_free_multiple(segments[i],segSize[i]);
  }
  addedCnt=0;
  for(idx=0 ;  ; idx++)
  {
    if(idx<segcnt && idx%10 ==0 )
      continue;

    segments[idx] = palloc_get_multiple(PAL_USER,segSize[idx] = random_ulong()%8+1);//get 1~8 random pagecd
    if(segments[idx] == NULL ) break;
    addedCnt++;
    if(i%10 == 0)
      continue;
  }
  elapsed_time = timer_ticks() - start_time;//save timer status.

  for(i=0;;i++)
  {
    if(segments[i]==NULL) break;
    palloc_free_multiple(segments[i],segSize[i]);
  }
  printf("fragmentation bench finished, (with seed: %d)\n", seed);
  printf("segment allocatied: initial - %d, final - %d\n", segcnt, addedCnt+segcnt/10);
  printf("fragmentation score: %d/%d(%d%%)\n\n",addedCnt,segcnt-segcnt/10,(addedCnt*100/(segcnt-segcnt/10)));

  free(segments);
  free(segSize);

  return addedCnt*100 / (segcnt - segcnt/10) ;
}
