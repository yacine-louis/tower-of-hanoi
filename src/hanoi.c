#include "stack.h"
#include "hanoi.h"

void moveRecursive(Node **src, Node **dest)
{
  // get the the disk from the source stack
  int disk = pop(src);
  if (disk == -1)
    return; // nothing to move

  // push the disk to destination stack
  push(dest, disk);
}

void hanoiRecursive(int n, Node **A, Node **C, Node **B)
{
  if (n == 0)
    return;

  hanoiRecursive(n - 1, A, B, C); // Move n-1 disks from A to B
  moveRecursive(A, C);            // Move nth disk from A to C
  hanoiRecursive(n - 1, B, C, A); // Move n-1 disks from B to C
}

void moveIterative(Node **src, Node **dest)
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

void hanoiIterative(int n, Node **A, Node **B, Node **C)
{
  // pointer to stacks
  Node **src = A;  // Source peg (starting position)
  Node **dest = C; // Destination peg (target position)
  Node **aux = B;  // Auxiliary peg (helper)

  int totalMoves = (1 << n) - 1; // 2^n - 1

  for (int i = 1; i <= totalMoves; i++)
  {
    if (n % 2 == 1) // Odd number of disks
    {
      if (i % 3 == 1)
        moveIterative(src, dest); // A <-> C
      else if (i % 3 == 2)
        moveIterative(src, aux); // A <-> B
      else
        moveIterative(aux, dest); // B <-> C
    }
    else // Even number of disks
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
