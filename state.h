#include <stdint.h>

#define FIELD_X 4
#define FIELD_Y 3
#define INAROW 2
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef uint8_t FIELD[FIELD_X][FIELD_Y];
typedef struct STATE{
    FIELD field;
    int figure;
    struct STATE *prev;
} STATE;
