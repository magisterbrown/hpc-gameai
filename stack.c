#define NOB_IMPLEMENTATION
#include "nob.h"
#include <stdio.h>

typedef struct {
    int val;
} Horizont;

#define STACK_BLOCK 1 
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
    if(prev != NULL)
        prev->next = new;

    return new;
}

Block *stack_push(Block *stack, Horizont *el)
{
    if(stack->top == STACK_BLOCK)
    {
        if(stack->next != NULL){
            stack = stack->next;
        }
        else
            stack = stack_block_alloc(stack);
    }
    memcpy(&stack->elements[stack->top], el, sizeof(Horizont));
    stack->top++;
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
    if(stack->top == 0)
        return NULL;
    stack->top--; 
    memcpy(el, &stack->elements[stack->top], sizeof(Horizont));
    if(stack->top == 0 && stack->prev!=NULL)
        return stack->prev;
    return stack;
}

void stack_free(Block* start)
{
    Block *next;
    while(start != NULL)
    {
        next = start->next;
        free(start);
        start = next;
    }

}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    Block *start = stack_block_alloc(NULL);
    Block *end = start;
    for(int i=-1;i<30;i++)
    {
        Horizont hor = {i};
        end = stack_push(end, &hor);
    }
    Horizont hor = {0};
    while(1) {
        end = stack_pop(end, &hor);
        if(end == NULL)
            break;
        printf("%d\n", hor.val);
    }
    end = start;
    for(int i=-1;i<50;i++)
    {
        Horizont inp = {i};
        end = stack_push(end, &inp);
    }
    while(1) {
        Horizont *peeked = stack_peek(end);
        end = stack_pop(end, &hor);
        if(end == NULL)
            break;
        assert(peeked->val == hor.val);
    }
    stack_free(start);
    printf("Test cases\n");    

    return 0;
}
