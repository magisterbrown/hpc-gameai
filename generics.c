#define NOB_IMPLEMENTATION
#include "nob.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    Nob_String_Builder sb = {0};
    nob_read_entire_file("result.template", &sb);
    nob_sb_append_null(&sb);


    Nob_String_Builder directions = {0};
    for(int i=0;i<4;i++){
        char* direct = "    {.point={0, 0}, .dir={0, 1}, .length=2},\n";
        nob_da_append_many(&directions, direct, strlen(direct));
    }
    nob_sb_append_null(&directions);
    char *content = nob_temp_sprintf(sb.items, directions.items);
    nob_write_entire_file("shared/result.h", content, strlen(content)); 
    return 0;
}
