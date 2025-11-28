#include <stdio.h>
#include "exetime.h"

// function to initialize start time
void startTimer(Timer *t)
{
  t->start = clock();
}

// function to initialize end time
void endTimer(Timer *t)
{
  t->end = clock();
}

// function to calculate elapsed time in seconds
void getElapsedTime(Timer *t)
{
  return ((double)(t->end - t->start)) / CLOCKS_PER_SEC;
}
