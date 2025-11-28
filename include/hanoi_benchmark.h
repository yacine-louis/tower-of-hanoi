#ifndef HANOI_BENCHMARK_H
#define HANOI_BENCHMARK_H
/**
 * Core benchmarking data structures and logic
 */

#define MAX_DISKS 35

// `BenchmarkResult` represent a row in result table
typedef struct
{
  int n;           // number of disk
  double time_sec; // time in seconds
  long moves;      // number of moves
} BenchmarkResult;

// `BenchmarkData` represent all result table
typedef struct
{
  BenchmarkResult results[MAX_DISKS]; // table rows
  int count;                          // number of benchmarks
  char method[20];                    // used method(recursive/iterative)
} BenchmarkData;

// benchmark execution functions

void runSingleTest();
void runBenchmarkSuite();

#endif