#define NOB_IMPLEMENTATION
#include "nob.h"
#include <string.h>

void cflags(Nob_Cmd *cmd)
{
    nob_cmd_append(cmd, "-Wall", "-Wextra", "-ggdb");
}

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char *inputs[] = {
        //"search.c",
        //"graph.c"
        "play.c"
    };
    Nob_Cmd cmd = {0};

    nob_cmd_append(&cmd, "cc", "-o", "run");
    cflags(&cmd);
    nob_da_append_many(&cmd, inputs, NOB_ARRAY_LEN(inputs));
    if(nob_cmd_run_sync(cmd)){
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, "./run");
        nob_cmd_run_sync(cmd);
    }
    
    return 0;
}
