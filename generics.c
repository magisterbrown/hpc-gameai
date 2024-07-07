#define NOB_IMPLEMENTATION
#include "nob.h"
#include "state.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    Nob_String_Builder sb = {0};
    nob_read_entire_file("result.template", &sb);
    nob_sb_append_null(&sb);

    Nob_String_Builder directions = {0};
    for(int i=0;i<FIELD_X;i++)
    {
        char* direct = nob_temp_sprintf("    {.point={%d, 0}, .dir={0, 1}, .length=FIELD_Y},\n", i);
        nob_da_append_many(&directions, direct, strlen(direct));
    }
    for(int i=0;i<FIELD_Y;i++)
    {
        char* direct = nob_temp_sprintf("    {.point={0, %d}, .dir={1, 0}, .length=FIELD_X},\n", i);
        nob_da_append_many(&directions, direct, strlen(direct));
    }
    for(int i=1;i<FIELD_Y;i++)
    {
        int length = MIN(FIELD_Y-i, FIELD_X);
        if(INAROW>length)
            continue;
        char* direct = nob_temp_sprintf("    {.point={0, %d}, .dir={1, 1}, .length=%d},\n", i, length);
        nob_da_append_many(&directions, direct, strlen(direct));
    }
    for(int i=0;i<FIELD_X;i++)
    {
        int length = MIN(FIELD_X-i, FIELD_Y);
        if(length >= INAROW)
        {
            char* direct = nob_temp_sprintf("    {.point={%d, 0}, .dir={1, 1}, .length=%d},\n", i, length);
            nob_da_append_many(&directions, direct, strlen(direct));
        }
        length = MIN(i+1, FIELD_Y);
        if(length >= INAROW)
        {
            char* direct = nob_temp_sprintf("    {.point={%d, 0}, .dir={-1, 1}, .length=%d},\n", i, length);
            nob_da_append_many(&directions, direct, strlen(direct));
        }

    }
    for(int i=1;i<FIELD_Y;i++)
    {
        int length = MIN(FIELD_Y-i, FIELD_X);
        if(INAROW>length)
            continue;
        char* direct = nob_temp_sprintf("    {.point={%d, %d}, .dir={-1, -1}, .length=%d},\n", FIELD_X-1, i, length);
        nob_da_append_many(&directions, direct, strlen(direct));
    }
    nob_sb_append_null(&directions);
    char *content = nob_temp_sprintf(sb.items, directions.items);
    nob_write_entire_file("shared/result.h", content, strlen(content)); 
    return 0;
}
