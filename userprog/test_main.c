#include <stdio.h>
#include <random.h>
#include "devices/timer.h"
#include "threads/palloc.h"
#include "userprog/test_main.h"

void
palloc_test_main (void) 
{
  int elapsedtmp = 0;
  int i;
  //test_name = argv[0];

  printf ("begin testing\n");
  printf("\n");
  printf ("begin testing with seed 1~100\n");
  
  for( i =0;i<100;i++)
    elapsedtmp +=  testpalloc_add100_remove50_add50_remove100(1);//seeded test, to should have stable outcome
  printf ("ended test, elapsed time : %d.%04d seconds\n", elapsedtmp/1000, elapsedtmp%1000);
  return 0;

}

int testpalloc_add100_remove50_add50_remove100(int seed)
{
  int start_time = timer_ticks();
  int elapsed_time;
  void* segments[100];
  int segSize[100];
  int i;

  random_init (seed);


  for( i =0;i<100;i++)//add 100 random
    segments[i] = palloc_get_multiple(PAL_USER,segSize[i] = random_ulong()%16+1);//get 1~16 random pagecd

  for(i = 0;i<100;i+=2)//remove 
    palloc_free_multiple(segments[i],segSize[i]);

  for(i = 0;i<100;i+=2)//add 50, random
    segments[i] = palloc_get_multiple(PAL_USER,segSize[i] = random_ulong()%16+1);//get 1~16 random page

  elapsed_time = timer_ticks() - start_time;//save timer status.

  //todo: check segmentation here
  


  for(i = 0;i<100;i++)
    palloc_free_multiple(segments[i],segSize[i]);

  

  return elapsed_time;
}
