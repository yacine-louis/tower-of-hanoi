#ifndef HANOI_BENCHMARK_H
#define HANOI_BENCHMARK_H

// benchmarking data structures

// structure to represent a row in result table
typedef struct
{
  int n;           // number of disk
  double time_sec; // time in seconds
} BenchmarkResult;

// structure to represent all result table
typedef struct
{
  BenchmarkResult *results; // table rows
  int resultsLength;        // result table length
  char method[20];          // used method(recursive/iterative)
} BenchmarkData;

// benchmark functions

// run single test
void runSingleTest();

// run multiple test
void runBenchmarkSuite();

#endif