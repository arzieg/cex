#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int num;
    struct node *next;
} Node, *NodePtr;

void printList(NodePtr);
NodePtr List(int);

int main()
{

    int n;
    NodePtr top, np, last;
    top = NULL;
    printf("\nEnter a number: ");
    if (scanf("%d", &n) != 1)
        n = 0;
    while (n != 0)
    {
        np = List(n); // creates a new node
        if (top == NULL)
            top = np; // set top if first node
        else
            last->next = np; // set last -> next for other nodes
        last = np;           // update last to new node

        printf("\nEnter more numbers:");
        if (scanf("%d", &n) != 1)
            n = 0;
    }
    printList(top);
}

NodePtr List(int n)
{
    NodePtr np = (NodePtr)malloc(sizeof(Node));
    if (np == NULL)
        exit(EXIT_FAILURE);
    np->num = n;
    np->next = NULL;
    return np;
}

void printList(NodePtr np)
{
    while (np != NULL)
    {
        printf("%d\n", np->num);
        np = np->next;
    }
}