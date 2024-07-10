#define NOB_IMPLEMENTATION
#include "nob.h"
#include <stdio.h>

typedef struct {
    int val;
} Horizont;

#define STACK_BLOCK 1024
typedef struct Block {
    Horizont elements[STACK_BLOCK];
    int top;
    struct Block *prev;
    struct Block *next;
} Block;


Block *stack_block_alloc(Block *prev)
{
    Block *new = malloc(sizeof(Block));
    new->top = 0;
    new->next = NULL;
    new->prev = prev;

    return new;
}

Block *stack_push(Block *stack, Horizont *el)
{
    memcpy(&stack->elements[stack->top], el, sizeof(Horizont));
    stack->top++;
    assert(stack->top<1024);
    return stack;
}

Horizont *stack_peek(Block *stack)
{
    if(stack->top == 0)
        return NULL;
    return &stack->elements[stack->top-1];
}

Block *stack_pop(Block *stack, Horizont *el)
{
    if(stack->top==0)
        return NULL;    
    stack->top--; 
    memcpy(el, &stack->elements[stack->top], sizeof(Horizont));
    return stack;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    printf("You got stacked twice\n");    
    return 0;
}
