#ifndef EXETIME_H
#define EXETIME_H

#include <time.h>

/**
 * The `Timer` struct measures execution time by recording a start timestamp,
 * an end timestamp, and computing the difference between them.
 */
typedef struct
{
  clock_t start;
  clock_t end;
} Timer;

// functions prototype

// initialize start time
void startTimer(Timer *t);

// initialize end time
void endTimer(Timer *t);

// calculate elapsed time in seconds
double getElapsedTime(Timer *t);

#endif