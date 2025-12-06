#include <stdio.h>
#include <string.h>
#include "hanoi_benchmark.h"
#include "stack.h"
#include "exetime.h"
#include "hanoi.h"
#include "hanoi_visualizer.h"

void runSingleTest()
{
  int n, method;

  printf("\n--------- Single Test ---------\n");

  // validate number of disks, it must be integer and positive
  while (true)
  {
    printf("Enter number of disks: ");
    if (scanf("%d", &n) == 1 && n > 0)
      break;

    printf("invalid number of disks!\n");

    // not valid number of disks, clear input buffer
    while (getchar() != '\n')
      ;
  }

  printf("\nSelect method:\n");
  printf(" 1. recursive\n");
  printf(" 2. iterative\n");

  // validate method, it must be integer and either 1 or 2
  while (true)
  {
    printf("Choice: ");
    if (scanf("%d", &method) == 1 && (method == 1 || method == 2))
      break;

    printf("invalid method! enter 1 or 2\n");

    // not valid method, clear input buffer
    while (getchar() != '\n')
      ;
  }

  Stack *A = createStack();
  Stack *B = createStack();
  Stack *C = createStack();

  if (!A || !B || !C)
  {
    printf("failed to create stacks\n");
    freeStack(A);
    freeStack(B);
    freeStack(C);
    return;
  }

  // init peg A
  for (int i = n; i >= 1; i--)
    push(A, i);

  printf("\n--- Initial state ---\n");
  printf("Peg A: ");
  printStack(A);
  printf("Peg B: ");
  printStack(B);
  printf("Peg C: ");
  printStack(C);

  Timer t;
  startTimer(&t);

  if (method == 1)
  {
    printf("\n--- Running recursive method ---\n");
    hanoiRecursive(n, A, C, B);
  }
  else
  {
    printf("\n--- Running Iterative method ---\n");
    hanoiIterative(n, A, B, C);
  }

  endTimer(&t);

  printf("\n--- Final state ---\n");
  printf("Peg A: ");
  printStack(A);
  printf("Peg B: ");
  printStack(B);
  printf("Peg C: ");
  printStack(C);

  // free stack memory
  freeStack(A);
  freeStack(B);
  freeStack(C);

  printf("\n--- Results ---\n");
  printf("Number of disks: %d\n", n);
  printf("Method: %s\n", method == 1 ? "Recursive" : "Iterative");
  printf("Execution time: %.5f (sec)", getElapsedTime(&t));

  printf("\nPress Enter to continue...");
  getchar();
  getchar();
}

void runBenchmarkSuite()
{
  int start_n, end_n, method;
  printf("\n--------- Range Test ---------\n");

  // validate starting number, it must be integer and positive
  while (true)
  {
    printf("Enter starting number of disks: ");
    if (scanf("%d", &start_n) == 1 && start_n > 0)
      break;

    printf("invalid starting number of disks! try again...\n");

    // not valid starting number, clear input buffer
    while (getchar() != '\n')
      ;
  }

  // validate ending number, it must be integer and bigger than starting number
  while (true)
  {
    printf("Enter ending number of disks: ");
    if (scanf("%d", &end_n) == 1)
    {
      if (end_n <= start_n)
      {
        printf("ending number must be greater than staring number\n");
      }
      else
      {
        break;
      }
    }
    else
    {
      printf("invalid ending number of disks! try again...\n");
    }

    // not valid starting number, clear input buffer
    while (getchar() != '\n')
      ;
  }

  printf("Select method:\n");
  printf(" 1. recursive\n");
  printf(" 2. iterative\n");

  // validate method, it must be integer and either 1 or 2
  while (true)
  {
    printf("Choice: ");
    if (scanf("%d", &method) == 1 && (method == 1 || method == 2))
      break;

    printf("invalid method! enter 1 or 2\n");

    // not valid method, clear input buffer
    while (getchar() != '\n')
      ;
  }

  // structure to store the benchmark result
  BenchmarkData data;
  data.results = (BenchmarkResult *)malloc((end_n - start_n + 1) * sizeof(BenchmarkResult));

  // check if allocation failed
  if (data.results == NULL)
  {
    printf("\nallocating benchmark results failed");
    return;
  }
  // init result length
  data.resultsLength = end_n - start_n + 1;
  strcpy(data.method, method == 1 ? "Recursive" : "Iterative");

  printf("\n--- Running Benchmark ---\n");
  printf("%-8s %-15s\n", "Disks", "Time (sec)");
  printf("----------------------\n");

  for (int n = start_n; n <= end_n; n++)
  {
    Stack *A = createStack();
    Stack *B = createStack();
    Stack *C = createStack();

    if (!A || !B || !C)
    {
      printf("failed to create stacks\n");
      freeStack(A);
      freeStack(B);
      freeStack(C);
      return;
    }

    // initialize stack A
    for (int i = n; i >= 1; i--)
      push(A, i);

    Timer t;
    startTimer(&t);

    if (method == 1)
    {
      hanoiRecursive(n, A, C, B);
    }
    else
    {
      hanoiIterative(n, A, B, C);
    }

    endTimer(&t);
    // get elapsed time
    double elapsed = getElapsedTime(&t);
    // fill result data
    int index = n - start_n;
    data.results[index].n = n;
    data.results[index].time_sec = elapsed;

    printf("%-8d %-15.5f\n", n, elapsed);

    // free memory
    freeStack(A);
    freeStack(B);
    freeStack(C);
  }

  printf("\nBenchmark complete!\n");
  printf("\nOpening visualization window...\n");

  visualizeResults(&data);

  printf("\nPress Enter to continue...");
  getchar();
  getchar();

  // free dynamic result array
  free(data.results);
  data.results = NULL;
}
