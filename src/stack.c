#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Node *createNode(int data)
{
  // create new node
  Node *newNode = (Node *)malloc(sizeof(Node));

  // check if allocation failed
  if (newNode == NULL)
    return NULL;

  // initialize the node data
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

Stack *createStack()
{
  // allocate space for stack
  Stack *stack = (Stack *)malloc(sizeof(Stack));

  // check if allocation failed
  if (stack == NULL)
    return NULL;

  // init new stack
  stack->pTop = NULL;
  return stack;
}

int isEmpty(Stack *stack)
{
  // we must check that src != NULL first because *(NULL) will result in a bad pointer
  return stack == NULL || stack->pTop == NULL;
}

int push(Stack *stack, int data)
{
  // ensure that stack is initialized
  if (stack == NULL)
    return -1;

  // create and initialize new node
  Node *newNode = createNode(data);
  if (!newNode)
    return -1;

  // attach node to the head of linked-list
  newNode->next = stack->pTop;
  stack->pTop = newNode;
  return 0;
}

int pop(Stack *stack)
{
  // ensure that the stack is not empty
  if (isEmpty(stack))
    return -1;

  // remove the node at the head of linked-list
  int topData = stack->pTop->data;

  // point to the head of linked-list
  Node *temp = stack->pTop;
  stack->pTop = stack->pTop->next;

  // free memory
  free(temp);
  return topData;
}

int peek(Stack *stack)
{
  // ensure the stack is not empty
  if (isEmpty(stack))
    return -1;

  return stack->pTop->data;
}

void freeStack(Stack *stack)
{
  // make sure that the stack has been initialized
  if (stack == NULL)
    return;

  // empty the stack after finishing
  Node *current = stack->pTop;
  while (current != NULL)
  {
    Node *next = current->next;
    free(current);
    current = next;
  }

  // free stack structure
  free(stack);
}

void printStack(Stack *stack)
{
  // assure the stack is not empty
  if (isEmpty(stack))
  {
    printf("\n");
    return;
  }

  Node *temp = stack->pTop;
  // loop through the linked-list
  while (temp != NULL)
  {
    printf("%d", temp->data);

    // we don't print (->) in last element
    if (temp->next)
      printf(" -> ");

    temp = temp->next;
  }
  printf("\n");
}
