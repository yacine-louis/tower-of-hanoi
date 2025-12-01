#include "stack.h"
#include "hanoi.h"

#include <stdio.h>

void moveRecursive(Stack *src, Stack *dest)
{
  // assure the source stack is not empty
  if (isEmpty(src))
    return;

  // move disk from source to destination
  int disk = pop(src);
  push(dest, disk);
}

void hanoiRecursive(int n, Stack *A, Stack *C, Stack *B)
{
  if (n == 0)
    return;

  hanoiRecursive(n - 1, A, B, C); // move n-1 disks from A to B
  moveRecursive(A, C);            // move nth disk from A to C
  hanoiRecursive(n - 1, B, C, A); // move n-1 disks from B to C
}

void moveIterative(Stack *src, Stack *dest)
{
  // check the value on the top of the stack of source and destination
  int srcTop = peek(src);
  int destTop = peek(dest);

  if (srcTop == -1)
  {
    // source stack is empty, so move disk from destination to source
    int disk = pop(dest);
    push(src, disk);
  }
  else if (destTop == -1)
  {
    // destination stack is empty, so move disk from source to destination
    int disk = pop(src);
    push(dest, disk);
  }
  else if (srcTop < destTop)
  {
    // source has smaller disk, move it to destination
    int disk = pop(src);
    push(dest, disk);
  }
  else
  {
    // destination has smaller disk, move it to source
    int disk = pop(dest);
    push(src, disk);
  }
}

void hanoiIterative(int n, Stack *A, Stack *B, Stack *C)
{
  //  for n disks, there are 2^n - 1 total moves
  //  - odd n: (A->C, A->B, B->C)
  //  - even n: (A->B, A->C, B->C)

  // pointer to stacks
  Stack *src = A;
  Stack *dest = C;
  Stack *aux = B;

  int totalMoves = (1 << n) - 1; // 2^n - 1

  for (int i = 1; i <= totalMoves; i++)
  {
    // odd number of disks
    if (n % 2 == 1)
    {
      if (i % 3 == 1)
        moveIterative(src, dest); // A <-> C
      else if (i % 3 == 2)
        moveIterative(src, aux); // A <-> B
      else
        moveIterative(aux, dest); // B <-> C
    }
    else // even number of disks
    {
      if (i % 3 == 1)
        moveIterative(src, aux); // A <-> B
      else if (i % 3 == 2)
        moveIterative(src, dest); // A <-> C
      else
        moveIterative(aux, dest); // B <-> C
    }
  }
}
