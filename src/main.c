#include <stdio.h>
#include "hanoi_benchmark.h"

int main()
{
  int choice;

  while (1)
  {
    printf("------------------------------\n");
    printf("-       Tower Of Hanoi       -\n");
    printf("------------------------------\n");
    printf(" 1. Run single test\n");
    printf(" 2. Run range test (visualize Graph)\n");
    printf(" 3. Exit program\n");
    printf("\n Enter your choice: ");

    if (scanf("%d", &choice) != 1)
    {
      // not an integer, clear the keyboard buffer
      while (getchar() != '\n')
        ;
      printf("Invalid input! enter a number.\n");
      continue;
    }

    switch (choice)
    {
    case 1:
      runSingleTest();
      break;
    case 2:
      runBenchmarkSuite();
      break;
    case 3:
      printf("\nExiting program...\n");
      return 0;
    default:
      printf("Invalid choice! try again.\n");
      printf("Press Enter to continue...");
      getchar();
      getchar();
    }
  }

  return 0;
}