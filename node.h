typedef struct Node {
    uint8_t field[FIELD_X][FIELD_Y];
    uint8_t figure;
    struct Node *children[FIELD_X];
} Node;
