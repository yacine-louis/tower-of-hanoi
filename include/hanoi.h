#ifndef HANOI_H
#define HANOI_H

#include <stdlib.h>
#include "stack.h"

// functions prototype

// move disks from source stack to destination stack (recursive version)
void moveRecursive(Stack *src, Stack *dest);

// solves tower of hanoi recursively
void hanoiRecursive(int n, Stack *A, Stack *C, Stack *B);

// move disks from source stack to destination stack (iterative version)
void moveIterative(Stack *src, Stack *dest);

// solves tower of hanoi iteratively
void hanoiIterative(int n, Stack *A, Stack *B, Stack *C);

#endif