#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "shared/result.h"
#define ARENA_IMPLEMENTATION 
#include "external/arena.h"
#include <string.h>

#define clear() printf("\033[H\033[J")

static inline void print_field(FIELD *field) {
    for(int y=0;y<FIELD_Y;y++) {
        for(int x=0;x<FIELD_X;x++)
            printf("%d ", (*field)[x][y]);
        printf("\n");
    }
}

int make_move(FIELD *dst, FIELD *src, int move, uint8_t fig) 
{
    for(int row=0;row<FIELD_Y-1;row++){
        if((*src)[move][row] == 0 && (*src)[move][row+1] != 0)
        {
            (*dst)[move][row] = fig;
            return 0;
        }
    }
    (*dst)[move][FIELD_Y-1] = fig;
    return 0;
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int longest(FIELD *field, Direction arow, int fig) {
    int res = 0;
    int curr = 0;
    for(int i=0;i<arow.length;i++){
        int comp = (*field)[arow.point.x][arow.point.y] == fig;
        curr = (curr+comp)*comp;
        res = MAX(res, curr);
        arow.point.x+=arow.dir.x; 
        arow.point.y+=arow.dir.y;
    }
    return res;
}

int is_win(FIELD *field, int figure){
    for(size_t i=0;i<sizeof(dirs)/sizeof(dirs[0]);i++)
        if(longest(field, dirs[i], figure) >= INAROW)
            return 1;
    return 0;
}

#define next_fig(fig)  2-(fig>>1)
typedef struct Node{
    struct Node *children[FIELD_X];
    struct Node *parrent;
    int figure;
    int depth;
    int value;
} Node;

typedef struct {
    FIELD field;    
    Node *intree;
} Horizont;

#define STACK_BLOCK 1024
typedef struct Block {
    Horizont elements[STACK_BLOCK];
    int top;
    struct Block *prev;
    struct Block *next;
} Block;

//#define push_stack(stack, 

//Block *push_stack(Block *stack, Node *tree, FIELD *field)
//{
//    Block *res = stack;
//    if(stack == NULL)
//    {
//        res = malloc(sizeof(Block));
//        res->top = 0;
//        res->prev = NULL;
//        res->next = NULL;
//    }
//    else if(stack->top==STACK_BLOCK-1)
//    {
//        res = stack->next;
//        if(res==NULL) 
//            res = malloc(sizeof(Block));
//        stack->next = res;
//        res->top = 0;
//        res->prev = stack;
//        res->next = NULL;
//    }
//    else
//        res->top++;
//    Horizont *new = &res->elements[res->top];
//    new->intree = tree;
//    memcpy(&new->field, field, sizeof(FIELD));
//    return res;
//}

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
    printf("Stack top %d\n", stack->top);
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

int is_longest(FIELD *field, int figure){
    int res = 0;
    for(size_t i=0;i<sizeof(dirs)/sizeof(dirs[0]);i++)
        res = MAX(res, longest(field, dirs[i], figure));
    return res;
}
#define illegal_move(field, move) field[move][0] != 0 || move < 0 || move >= FIELD_X

#define NO_VALUE -100

void record_value(Node *leaf, int value)
{
    while(leaf!=NULL){
        if(leaf->value==NO_VALUE)
            leaf->value = value;
        else if((value>leaf->value && leaf->figure == 1) || (value<leaf->value && leaf->figure == 2 ))
            leaf->value = value;
        else
            break;
        leaf = leaf->parrent;
    } 
}

int agi(FIELD *field, int figure) 
{
    static Arena nodes = {0};
    Node *root = arena_alloc(&nodes, sizeof(Node));
    root->figure = figure;
    root->parrent = NULL;
    root->depth = 0;
    root->value = NO_VALUE;

    Horizont *first = malloc(sizeof(Horizont));
    first->intree = root;
    memcpy(&first->field, field, sizeof(FIELD));

    Block *bottom = stack_block_alloc(NULL);
    Block *top = stack_push(bottom, first);
    int j=0;
    while(j++<300)
    {
        Horizont oldest = {0};
        top = stack_pop(top, &oldest);
        if(top==NULL)
            break;
        //Horizont *oldest = first;
        Node *parrent = oldest.intree;
        if(is_win(&oldest.field, parrent->figure))
        {
            record_value(parrent, INAROW*(parrent->figure==1) - INAROW*(parrent->figure==2));
            print_field(&oldest.field);
            printf("Win %d\n", parrent->value);
            continue;
        }

        if(parrent->depth == 1)
        {
            //TODO: Evaluate position 
            record_value(parrent, is_longest(&oldest.field, 1) - is_longest(&oldest.field, 2));
            //print_field(&oldest.field);
            //printf("Heuristics %d\n", parrent->value);
            continue;
        }
        for(int i=0;i<FIELD_X;i++)
        {
            if(illegal_move(oldest.field, i)) //TODO: check for legal move
                continue;
            Node *child = arena_alloc(&nodes, sizeof(Node));
            child->figure = next_fig(parrent->figure);
            child->parrent = parrent;
            child->depth = parrent->depth+1;
            child->value = NO_VALUE;
            parrent->children[i] = child; //TODO: set to null missing child pointers
            
            stack_push(top, &oldest);
            Horizont *next = stack_peek(top);
            next->intree = child;
            make_move(&next->field, &next->field, i, child->figure);
        }
    }

    arena_free(&nodes);
    return root->value;
}



STATE *process(STATE *state, char input)
{
    clear();
    int lost = is_win(&state->field, state->figure);
    if(lost) 
        printf("%d has won\n", state->figure);

    STATE *next = state;
    switch(input)
    {
        case 'a':
            printf("AGI invoked\n");
            if(lost)
                break;
            printf("AGI suggests move: %d\n", agi(&state->field, next_fig(state->figure)));
            break;
        case 'b':
            if(state->prev == NULL)
                break;
            next = state->prev;
            free(state);
            break;
        default:
                int move = (int)input-49;
                if(illegal_move(state->field, move))
                {
                    printf("Illegal move\n");
                    break;
                }
                if(lost) 
                    break;
                next = malloc(sizeof(STATE));
                memcpy(&next->field, &state->field, sizeof(FIELD));
                next->prev = state;
                next->figure = next_fig(state->figure);

                make_move(&next->field, &next->field, move, next->figure);
            break;
    }

    print_field(&next->field);  
    return next;
}
