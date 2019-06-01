/* sort.c 

   Test program to sort a large number of integers.
 
   Intention is to stress virtual memory system.
 
   Ideally, we could read the unsorted array off of the file
   system, and store the result back to the file system! */
#include <stdio.h>

/* Size of array to sort. */
#define SORT_SIZE 128

int
main (void)
{
  /* Array to sort.  Static to reduce stack usage. */
  int *array;
  array = malloc(sizeof(int));

  printf ("sort exiting with code %d\n", array[0]);
  return array[0];
}
