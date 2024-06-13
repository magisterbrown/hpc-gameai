#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define FIELD_X 7
#define FIELD_Y 6
#define FIELD_SZ FIELD_X*FIELD_Y
#define print_field(field) for(int i=0;i<FIELD_SZ;i++){printf("%d ", field[i]); if(i%FIELD_X==FIELD_X-1) printf("\n"); }

#define DEPTH 3

#define QUEUE 50
#define BATCH_SIZE 4


struct Node {
    uint8_t *field;
    uint8_t figure;
    struct Node *children[FIELD_X];
};

int ipow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

void expand(int size, uint8_t *batch, uint8_t *figure, uint8_t *result, bool *valid);

int main(void) {
    // Load board:
    printf("Search\n");

    uint8_t field[FIELD_SZ];

    FILE *f = fopen("./fields/example", "rb");
    fseek(f, 0, SEEK_END);
    long m = ftell(f);
    char *buff = malloc(sizeof(char)*m);
    fseek(f, 0, SEEK_SET);
    fread(buff, m, 1, f);
    int pt = 0;
    for(int i=0;i<m;i++){
        int fig=2;
        switch (buff[i]){
            case '0':
                fig = 0;
                break;
            case '1':
                fig = 1;
                break;
            case '2':
                fig = 2;
                break;
            case '\n':
                continue;
            default:
                printf("Wrong symbol");
                return 1;
        }
        field[pt++] = fig;
    }

    if(f) fclose(f);

    // Tree Search:
    struct Node queue[QUEUE];
    uint8_t batch[FIELD_SZ*BATCH_SIZE];
    uint8_t result[FIELD_SZ*BATCH_SIZE*FIELD_X];
    uint8_t figure[BATCH_SIZE];
    struct Node *batch_nodes[BATCH_SIZE];
    bool valid[BATCH_SIZE*FIELD_X];

    memset(valid, false, BATCH_SIZE*FIELD_X);

    int head = 0;
    int tail = 0;
    struct Node root;
    root.field = field;
    root.figure = 1;

    queue[tail++%QUEUE] = root;
    assert(tail != head);
    
    int sz = 0;
    for(int i=0;i<BATCH_SIZE;i++){
        if(head == tail)
            break;
        sz++;
        struct Node *first = queue+(head++%QUEUE);
        batch_nodes[i] = first;
        figure[i] = first->figure;
        memcpy(batch+i*FIELD_SZ, first->field, FIELD_SZ);
    }

    print_field(batch);
    expand(sz, batch, figure, result, valid);
    for(int i=0;i<sz*FIELD_X;i++){
        if(!valid[i])
            continue;

        struct Node *new = malloc(sizeof(struct Node));
        struct Node *parent = batch_nodes[i/FIELD_X];
        new->figure = (2-(parent->figure>>1));
        new->field = malloc(sizeof(uint8_t)*FIELD_SZ);
        memcpy(new->field, result+i, FIELD_SZ);
        uint8_t chid = i & ((1 << FIELD_X)-1);
        parent->children[chid] = new;

        printf("%d chid\n", chid);

        queue[tail++%QUEUE] = *new;
        assert(tail != head);

    }
    printf("\n");
    print_field(result);
    print_field(result);
    
    return 0;
}

void expand(int size, uint8_t *batch, uint8_t *figure, uint8_t *result, bool *valid) {
    for(int i=0;i<size;i++){
        uint8_t *curr_field=batch+i*FIELD_SZ;
        
        for(int j=0;j<FIELD_X;j++){
            if(curr_field[j] != 0)
                continue;

            valid[i*FIELD_X+j] = true;
            uint8_t *res_field = result+i*FIELD_SZ*FIELD_X+j*FIELD_SZ;
            memcpy(res_field, curr_field, FIELD_SZ);
            
            int cell, ncell;
            for(int k=0;k<FIELD_Y-1;k++){
                cell = j+FIELD_X*k;
                ncell = j+FIELD_X*(k+1);
                if(res_field[cell] == 0 && res_field[ncell] != 0)
                    res_field[cell] = figure[i];
            }
            if(res_field[ncell] == 0)
                res_field[j+(FIELD_Y-1)*FIELD_X] = figure[i];

        }
    }
}
