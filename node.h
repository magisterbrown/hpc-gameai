#include <stdint.h>

#include "external/arena.h"

#define FIELD_X 4
#define FIELD_Y 3
#define INAROW 2

typedef uint8_t FIELD[FIELD_X][FIELD_Y];

typedef struct Node {
    uint8_t field[FIELD_X][FIELD_Y];
    uint8_t figure;
    struct Node *children[FIELD_X];
} Node;

int search(Node *root, Arena *arena);

static inline void print_field(FIELD field) {
    for(int y=0;y<FIELD_Y;y++) {
        for(int x=0;x<FIELD_X;x++)
            printf("%d ", field[x][y]);
        printf("\n");
    }
}
