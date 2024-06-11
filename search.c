#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define FIELD_X 7
#define FIELD_Y 6
#define FIELD_SZ FIELD_X*FIELD_Y
//#define print_field(field) (for(intprintf("ss"))

int main(void) {
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
    

    for(int i=0;i<FIELD_SZ;i++){
        printf("%d ", field[i]);
        if(i%FIELD_X==FIELD_X-1)
            printf("\n");
    }

    if(f) fclose(f);
    return 0;
}
