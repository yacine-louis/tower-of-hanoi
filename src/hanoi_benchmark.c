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

  printf("----- Single Test -----\n");
  printf("Enter number of disks (1-%d): ", MAX_DISKS);
  scanf("%d", &n);

  if (n < 1 || n > MAX_DISKS)
  {
    printf("Invalid number of disks!\n");
    return;
  }

  printf("\nSelect method:\n");
  printf(" 1. Recursive\n");
  printf(" 2. Iterative\n");
  printf("Choice: ");
  scanf("%d", &method);

  if (method != 1 && method != 2)
  {
    printf("Invalid method!\n");
    return;
  }

  Stack *A = createStack();
  Stack *B = createStack();
  Stack *C = createStack();

  if (!A || !B || !C)
  {
    printf("Failed to create stacks.\n");
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
    printf("\n--- Running Recursive method ---\n");
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
  printf("Execution time: %f (sec)", getElapsedTime(&t));

  printf("\nPress Enter to continue...");
  getchar();
  getchar();
}

void runBenchmarkSuite()
{
  int method, start_n, end_n;

  printf("\n--- Range Test ---\n\n");

  printf("Select method:\n");
  printf(" 1. Recursive\n");
  printf(" 2. Iterative\n");
  printf("Choice: ");
  scanf("%d", &method);

  if (method != 1 && method != 2)
  {
    printf("Invalid method!\n");
    return;
  }

  printf("\nEnter starting number of disks (1-%d): ", MAX_DISKS);
  scanf("%d", &start_n);
  printf("Enter ending number of disks (%d-%d): ", start_n, MAX_DISKS);
  scanf("%d", &end_n);

  if (start_n < 1 || end_n > MAX_DISKS || start_n > end_n || (end_n - start_n + 1) > MAX_DISKS)
  {
    printf("Invalid range! ");
    if (start_n > end_n)
    {
      printf("Ending number must be greater than or equal to starting number.\n");
    }
    else if (start_n < 1)
    {
      printf("Starting number must be at least 1.\n");
    }
    else if (end_n > MAX_DISKS)
    {
      printf("Ending number must be at most %d.\n", MAX_DISKS);
    }
    else
    {
      printf("Range is too large (max %d tests).\n", MAX_DISKS);
    }
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
    return;
  }

  BenchmarkData data;
  data.count = 0;
  strcpy(data.method, method == 1 ? "Recursive" : "Iterative");

  printf("\n--- Running Benchmark ---\n");
  printf("%-8s %-15s %-15s\n", "Disks", "Moves", "Time (sec)");
  printf("-------------------------------------------------\n");

  for (int n = start_n; n <= end_n; n++)
  {
    Node *A = NULL, *B = NULL, *C = NULL;

    // initialize stack A
    for (int i = n; i >= 1; i--)
      push(&A, i);

    Timer t;
    startTimer(&t);

    if (method == 1)
    {
      hanoiRecursive(n, &A, &C, &B);
    }
    else
    {
      hanoiIterative(n, &A, &B, &C);
    }

    endTimer(&t);

    long moves = (1L << n) - 1;
    double elapsed = getElapsedTime(&t);

    data.results[data.count].n = n;
    data.results[data.count].time_sec = elapsed;
    data.results[data.count].moves = moves;
    data.count++;

    printf("%-8d %-15ld %-15.6f\n", n, moves, elapsed);

    // free memory
    while (A)
      pop(&A);
    while (B)
      pop(&B);
    while (C)
      pop(&C);
  }

  printf("\nBenchmark complete!\n");
  printf("\nOpening visualization window...\n");

  visualizeResults(&data);

  printf("\nPress Enter to continue...");
  getchar();
  getchar();
}