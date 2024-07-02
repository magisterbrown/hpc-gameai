#include <stdio.h>
#include "node.h"

#define clear() printf("\033[H\033[J")

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
        print_field(live);  
        char user = getchar();
        clear();
        switch(user){
            case 'e':
                return 0;
            case '\n':
                continue;
            case 'b':
                printf("Back\n");
                break;
            default:
                    illegal = make_move(&live, &live, (int)user-48, fig);
                    if(illegal)
                        printf("Illegal move\n");
                    fig = 2-(fig>>1);
                break;
        }
        
    }
}
