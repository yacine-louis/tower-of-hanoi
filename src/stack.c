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

int insertBeforeHead(Node **head, int data)
{
  // create/initialize new node
  Node *newNode = createNode(data);
  if (!newNode)
    return -1;

  // if linked list is empty
  if (*head == NULL)
  {
    // attach node to the head
    *head = newNode;
    return 0;
  }

  // attach node to the head of linked-list
  newNode->next = *head;
  *head = newNode;
  return 0;
}

int deleteHead(Node **head)
{
  // point to the head of linked-list
  Node *temp = *head;
  *head = (*head)->next;

  // free memory
  free(temp);
  return 0;
}

int isEmpty(Node **stack)
{
  // check the head of linked-list
  return *stack == NULL;
}

int push(Node **stack, int data)
{
  // insert the node at the head of the linked list
  if (insertBeforeHead(stack, data))
  {
    printf("Stack Overflow!\n");
    return -1;
  }
  return 0;
}

int pop(Node **stack)
{
  // check if stack is empty
  if (isEmpty(stack))
  {
    printf("Stack Underflow\n");
    return -1;
  }

  // remove the node at the head of linked-list
  int topData = (*stack)->data;
  deleteHead(stack);
  return topData;
}

int peek(Node **stack)
{
  // ensure the stack is not empty
  if (!isEmpty(stack))
    // return data stored in the top node
    return (*stack)->data;
  else
    return -1;
}

void printStack(Node **stack)
{
  Node *temp = *stack;
  // loop through the linked-list
  while (temp != NULL)
  {
    printf("%d", temp->data);
    if (temp->next)
    {
      // we don't print (->) in last element
      printf("-> ");
    }
    temp = temp->next;
  }
  printf("\n");
}
