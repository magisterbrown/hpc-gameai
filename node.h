#include <stdint.h>

#include "external/arena.h"

#define FIELD_X 7
#define FIELD_Y 6

typedef struct Node {
    uint8_t field[FIELD_X][FIELD_Y];
    uint8_t figure;
    struct Node *children[FIELD_X];
} Node;

int search(Node *root, Arena *arena);
