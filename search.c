#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define FIELD_X 7
#define FIELD_Y 6
#define FIELD_SZ FIELD_X*FIELD_Y
#define print_field(field) for(int i=0;i<FIELD_SZ;i++){printf("%d ", field[i]); if(i%FIELD_X==FIELD_X-1) printf("\n"); }

#define DEPTH 3

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
    
    print_field(field);

    if(f) fclose(f);

    // Tree Search:
    int tree = (ipow(FIELD_X, DEPTH)-1)/(FIELD_X-1);
    uint8_t* fields = malloc(sizeof(uint8_t)*FIELD_SZ*tree);
    float* votes = malloc(sizeof(float)*FIELD_SZ);
    bool* explored = malloc(sizeof(bool)*FIELD_SZ);
    explored[0] = true;
    memcpy(fields, field, FIELD_SZ);

    for(int d=0;d<DEPTH;d++){
        for(int j=0;j<( ipow(FIELD_X, d)-1)/(FIELD_X-1);j++){
            if(!explored[j] && explored[(j-1)/4]){
                // Do a turn
                memcpy(fields+j*FIELD_SZ, field, FIELD_SZ);
                explored[j] = true;
            }

        }
        int maxd = (ipow(FIELD_X, d)-1)/(FIELD_X-1);
        printf("Tree %d\n", maxd);    
    }

    printf("Tree %d\n", tree);    
    print_field(fields);

    free(fields);
    free(votes);
    free(explored);
    return 0;
}
