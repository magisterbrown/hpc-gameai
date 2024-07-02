#include "node.h"
#include <stdio.h>

#define ARENA_IMPLEMENTATION
#include "external/arena.h"

#define FIELDST FIELD_X*(FIELD_Y+1)

int main(void) {
    static Arena default_arena = {0}; 
    
    uint8_t field[FIELD_X][FIELD_Y];
    while(1) {
        Node *root = arena_alloc(&default_arena, sizeof(Node));
        memcpy(root->field, field, sizeof(field));
        search(root, &default_arena);
        print_field(root->field);
        int next_move;
        printf("You move:");
        scanf("%d", &next_move);
        memcpy(field, root->children[next_move]->field, sizeof(field));
        arena_free(&default_arena);
    }
    return 0;
}
