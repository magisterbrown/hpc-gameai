#define NOB_IMPLEMENTATION
#include "nob.h"
#include <stdio.h>

typedef struct {
    int val;
} Horizont;

#define STACK_BLOCK 1 
#include "stack.h"

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
