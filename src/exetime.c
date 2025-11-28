#include "exetime.h"

void startTimer(Timer *t)
{
  t->start = clock();
}

void endTimer(Timer *t)
{
  t->end = clock();
}

double getElapsedTime(Timer *t)
{
  // return the elapsed time
  return ((double)(t->end - t->start)) / CLOCKS_PER_SEC;
}
