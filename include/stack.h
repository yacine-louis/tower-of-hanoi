#ifndef STACK_H
#define STACK_H

// represent a node in a linked-list
typedef struct Node
{
  int data;
  struct Node *next;
} Node;

// stack is just a pointer to linked-list and points to the first element in linked-list
typedef struct
{
  Node *pTop;
} Stack;

// creates and initializes a new linked-list node
Node *createNode(int data);

// create and initialize new stack
Stack *createStack();

// check if stack is empty
int isEmpty(Stack *stack);

// add node to the stack
int push(Stack *stack, int data);

// remove node from the stack
int pop(Stack *stack);

// return the value at the top of the stack without removing it
int peek(Stack *stack);

// free stack memory
void freeStack(Stack *stack);

// print all elements of the stack from top to bottom
void printStack(Stack *stack);

#endif