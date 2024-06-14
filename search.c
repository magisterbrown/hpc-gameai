#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define ARENA_IMPLEMENTATION
#include "external/arena.h"

static Arena default_arena = {0};

#define FIELD_X 7
#define FIELD_Y 6

typedef struct Node {
    uint8_t field[FIELD_X][FIELD_Y];
    uint8_t figure;
    struct Node *children[FIELD_X];
} Node;

void *context_alloc(int size)
{
    return arena_alloc(&default_arena, sizeof(Node)*size);
}

#define QUEUE 700
Node *queue[QUEUE];
int head = 0, tail = 0;

void inqueue(Node *new)
{
    queue[head++%QUEUE] = new;
    assert(head-QUEUE<tail);
}

Node *dequeue()
{
    if(tail<head)
        return queue[tail++%head];
    return NULL;
}

#define BATCH 4
uint8_t batch_fields[BATCH][FIELD_X][FIELD_Y];
uint8_t batch_figures[BATCH];
Node *batch_nodes[BATCH];

bool valid[BATCH][FIELD_X];
uint8_t results[BATCH][FIELD_X][FIELD_X][FIELD_Y];

void batch_load(int i, uint8_t nfield[FIELD_X][FIELD_Y], uint8_t nfigure)
{
   memcpy(*batch_fields[i], nfield, FIELD_X*FIELD_Y);
   batch_figures[i] = nfigure;
}

void col_expand(int idx, int col)
{
    uint8_t (*input)[FIELD_X][FIELD_Y] = &batch_fields[idx]; 
    uint8_t (*output)[FIELD_X][FIELD_Y] = &results[idx][col]; 

    memcpy(output,input, FIELD_X*FIELD_Y);
    if((*input)[col][0] != 0)
        return;
    valid[idx][col] = true;
    for(int row=0;row<FIELD_Y-1;row++){
        if((*input)[col][row] == 0 && (*input)[col][row+1] != 0 ){
            (*output)[col][row] = batch_figures[idx];
            return;
        }
    }
    (*output)[col][FIELD_Y-1] = batch_figures[idx];
}

static inline void print_field(uint8_t field[FIELD_X][FIELD_Y]) {
    for(int y=0;y<FIELD_Y;y++) {
        for(int x=0;x<FIELD_X;x++)
            printf("%d ", field[x][y]);
        printf("\n");
    }
}

int main(void) {
    // Load board:
    printf("Search\n");
    
    Node *root = context_alloc(1);
    root->figure = 1;
    root->field[0][FIELD_Y-2] = 1;
    root->field[0][FIELD_Y-1] = 2;
    print_field(root->field);
    inqueue(root);

    for(int epoch=0;epoch<25;epoch++){
        memset(valid, false, sizeof(valid));
        //Send batch
        for(int i=0;i<BATCH;i++) {
            Node *next = dequeue();
            if(next == NULL)
                break;
            batch_load(i, next->field, next->figure);
            batch_nodes[i] = next;

            for(int col=0;col<FIELD_X;col++)
                col_expand(i, col);
        }

        //Process results
        for(int i=0;i<BATCH;i++){
            for(int col=0;col<FIELD_X;col++){
                if(valid[i][col]){
                    Node *parrent = batch_nodes[i];
                    Node *child = context_alloc(1);
                    child->figure = 2-(parrent->figure>>1);
                    memcpy(&child->field, results[i][col], FIELD_X*FIELD_Y);
                    parrent->children[col] = child;
                    inqueue(child);
                }
            }
        }
    }
    printf("\n");
    //print_field(root->children[0]->children[0]->field);
    printf("\n");
    //print_field(root->children[1]->field);
    print_field(results[0][0]);


    // Tree Search:
    
    arena_free(&default_arena);
    return 0;
}

