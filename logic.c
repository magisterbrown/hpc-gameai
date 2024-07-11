#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "shared/result.h"
#define ARENA_IMPLEMENTATION 
#include "external/arena.h"
#include <string.h>
#include <assert.h>

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

#define STACK_BLOCK 2
#include "stack.h"

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
    //TODO: make node constructor
    Node *root = arena_alloc(&nodes, sizeof(Node));
    memset(&root->children, 0, sizeof(root->children));
    root->figure = figure;
    root->parrent = NULL;
    root->depth = 0;
    root->value = NO_VALUE;

    Horizont first = {0};
    first.intree = root;
    memcpy(&first.field, field, sizeof(FIELD));

    Block *bottom = stack_block_alloc(NULL);
    Block *top = bottom;
    bottom = stack_push(bottom, &first);
    while(1)
    {
        Horizont oldest = {0};
        top = stack_pop(top, &oldest);
        if(top==NULL)
            break;
        Node *parrent = oldest.intree;
        int nfig = next_fig(parrent->figure);
        if(is_win(&oldest.field, nfig))
        {
            record_value(parrent, INAROW*(nfig==1) - INAROW*(nfig==2));
            continue;
        }

        if(parrent->depth == 4)
        {
            record_value(parrent, is_longest(&oldest.field, 1) - is_longest(&oldest.field, 2));
            continue;
        }
        for(int i=0;i<FIELD_X;i++)
        {
            if(illegal_move(oldest.field, i)) 
                continue;
            Node *child = arena_alloc(&nodes, sizeof(Node));
            //TODO: make node constructor
            child->figure = next_fig(parrent->figure);
            child->parrent = parrent;
            child->depth = parrent->depth+1;
            child->value = NO_VALUE;
            memset(&child->children, 0, sizeof(child->children));
            parrent->children[i] = child; 
            
            top = stack_push(top, &oldest);
            Horizont *next = stack_peek(top);
            next->intree = child;
            make_move(&next->field, &oldest.field, i, parrent->figure);
        }
    }

    for(int i=0;i<FIELD_X;i++) 
    {
        if(root->children[i] != NULL)
            printf("AGI thinks move %d is %d\n", i, root->children[i]->value);
    }
    int res = root->value;
    arena_free(&nodes);
    stack_free(bottom);
    return res;
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
