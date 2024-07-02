#include <stdio.h>
#include "node.h"

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
    memset(live, 0, sizeof(live));
    int move;
    int illegal = 0;
    int fig = 1;
    while(1)
    {
        if(illegal)
            printf("Illegal move\n");
        print_field(live);  
        scanf("%d", &move);
        printf("%d\n", move);
        illegal = make_move(&live, &live, move, fig);
        fig = 2-(fig>>1);
    }
}
