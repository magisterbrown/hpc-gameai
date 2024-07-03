#include <stdio.h>
#include <stdlib.h>
#include "node.h"

#define clear() printf("\033[H\033[J")
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct Real {
    FIELD state;
    struct Real *prev;
} Real;

typedef struct Vec2d {
    uint8_t x;
    uint8_t y;
} Vec2d;

typedef struct Direction {
    Vec2d point;
    Vec2d dir;
    uint8_t length;
} Direction;

int longest(FIELD *state, Direction arow, int fig) {
    int res = 0;
    int curr = 0;
    for(int i=0;i<arow.length;i++){
        int comp = (*state)[arow.point.x][arow.point.y] == fig;
        curr = (curr+comp)*comp;
        res = MAX(res, curr);
        arow.point.x+=arow.dir.x; 
        arow.point.y+=arow.dir.y;
    }
    return res;
}
int is_win(FIELD *state){
    for(int i=0;i<FIELD_X;i++) 
    {
        Direction arow = {{i, 0}, {0, 1}, FIELD_Y};
        if(longest(state, arow, 1) >= INAROW)
            return 1;
        if(longest(state, arow, 2) >= INAROW)
            return 2;
    }
    for(int i=0;i<FIELD_Y;i++) 
    {
        Direction arow = {{0, i}, {1, 0}, FIELD_X};
        if(longest(state, arow, 1) >= INAROW)
            return 1;
        if(longest(state, arow, 2) >= INAROW)
            return 2;
    }
    return 0;
}

int make_move(FIELD *dst, FIELD *src, int move, uint8_t fig) 
{
    if((*src)[move][0] != 0 || move < 0 || move >= FIELD_X)
        return 1;
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

//int invert_move(FIELD *dst, FIELD *src, int move) 
//{
//    return 0;
//}

int main(void)
{
    FIELD live; 
    Real *last;
    last = malloc(sizeof(Real));
    memset(live, 0, sizeof(live));
    memcpy(last->state, live, sizeof(FIELD));
    int fig = 1;
    int done = 0;
    while(1)
    {
        if(done)
            printf("Win for %d\n", done);
        print_field(live);  
        char user = getchar();
        clear();
        switch(user){
            case 'e':
                return 0;
            case 'b':
                if(last->prev == NULL)
                    break;
                printf("Back\n");
                Real *garbage = last;
                last = last->prev;
                memcpy(live, last->state, sizeof(FIELD));
                free(garbage);
                fig = 2-(fig>>1);
                done = 0;
                break;
            default:
                    if(done)
                        break;
                    if(make_move(&live, &live, (int)user-48, fig))
                        printf("Illegal move\n");
                    else{
                        Real *new;
                        new = malloc(sizeof(Real));
                        memcpy(new->state, live, sizeof(FIELD));
                        new->prev = last;
                        last = new;
                        fig = 2-(fig>>1);
                        done = is_win(&live);
                    }
                break;
        }
        while(getchar() != '\n') {}
    }
}
