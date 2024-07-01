#include "node.h"
#include <stdio.h>

#define ARENA_IMPLEMENTATION
#include "external/arena.h"

#define FIELDST FIELD_X*(FIELD_Y+1)

int main(void) {
    static Arena default_arena = {0}; 

    Node *root = arena_alloc(&default_arena, sizeof(Node));
    search(root, &default_arena);
    print_field(root->field);
    arena_free(&default_arena);

    return 0;
}
