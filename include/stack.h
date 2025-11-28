#ifndef STACK_H
#define STACK_H

/**
 * The `Node` struct represent a node in a linked-list
 */
typedef struct Node
{
  int data;
  struct Node *next;
} Node;

// functions prototype

// creates and initializes a new linked-list node
Node *createNode(int data);

// insert node before the head of the current linked-list
int insertBeforeHead(Node **head, int data);

// delete linked-list head
int deleteHead(Node **head);

// check if stack is empty
int isEmpty(Node **stack);

// add node to the stack
int push(Node **stack, int data);

// remove node from the stack
int pop(Node **stack);

// return the value at the top of the stack without removing it
int peek(Node **stack);

// print all elements of the stack from top to bottom
void printStack(Node **stack);

#endif