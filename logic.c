#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "shared/result.h"
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

int agi(FIELD *field, int figure) 
{
    return 0;
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
            printf("AGI suggests move: %d", agi(&state->field, state->figure));
            break;
        case 'b':
            if(state->prev == NULL)
                break;
            next = state->prev;
            free(state);
            break;
        default:
                int move = (int)input-49;
                if(state->field[move][0] != 0 || move < 0 || move >= FIELD_X)
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
