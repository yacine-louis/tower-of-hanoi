#ifndef HANOI_H
#define HANOI_H

#include "stack.h"

// functions prototype

// move disks from source stack to destination stack (recursively)
void moveRecursive(Node **src, Node **dest);

/**
 * Solves Tower of Hanoi recursively
 * Strategy: To move n disks from A to C using B as auxiliary:
 * 1. Move the top (n-1) disks from A to B (using C as auxiliary)
 * 2. Move the largest disk (nth disk) from A to C
 * 3. Move the (n-1) disks from B to C (using A as auxiliary)
 */
void hanoiRecursive(int n, Node **A, Node **C, Node **B);

// move disks from source stack to destination stack (iteratively)
void moveIterative(Node **src, Node **dest);

/**
 * Solves Tower of Hanoi iteratively using a pattern-based approach
 * Algorithm: For n disks, there are 2^n - 1 total moves
 * - Odd n: repeat (A->C, A->B, B->C)
 * - Even n: repeat (A->B, A->C, B->C)
 */
void hanoiIterative(int n, Node **A, Node **B, Node **C);

#endif