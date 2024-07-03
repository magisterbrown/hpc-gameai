#include <stdio.h>
#include <stdlib.h>
#include "node.h"

#define clear() printf("\033[H\033[J")

typedef struct Real {
    FIELD state;
    struct Real *prev;
} Real;

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
    while(1)
    {
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
                break;
            default:
                    if(make_move(&live, &live, (int)user-48, fig))
                        printf("Illegal move\n");
                    else{
                        Real *new;
                        new = malloc(sizeof(Real));
                        memcpy(new->state, live, sizeof(FIELD));
                        new->prev = last;
                        last = new;
                        fig = 2-(fig>>1);
                    }
                break;
        }
        while(getchar() != '\n') {}
    }
}
