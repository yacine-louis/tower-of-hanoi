#include <stdio.h>
#include "hanoi_cli.h"
#include "hanoi_benchmark.h"

int main()
{
  int choice;

  while (1)
  {

    printMenu();

    if (scanf("%d", &choice) != 1)
    {
      while (getchar() != '\n')
        ; // Clear input buffer
      printf("Invalid input! Please enter a number.\n");
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
      printf("\nExiting...\n");
      return 0;
    default:
      printf("Invalid choice! Please try again.\n");
      printf("Press Enter to continue...");
      getchar();
      getchar();
    }
  }

  return 0;
}