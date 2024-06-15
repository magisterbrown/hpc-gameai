#define NOB_IMPLEMENTATION
#include "nob.h"
#include <string.h>

void cflags(Nob_Cmd *cmd)
{
    nob_cmd_append(cmd, "-Wall", "-Wextra", "-ggdb", "-fno-stack-protector");
}

void visualize(Nob_Cmd *cmd)
{
    char *libdirs[] = {
        "cgraph",
        "gvc",
        "pathplan",
        "cdt"

    };
    char *path = "-I/home/brownie/random/graphviz/lib/%s";
    for(int i=0;i<NOB_ARRAY_LEN(libdirs);i++) {
        nob_cmd_append(cmd, nob_temp_sprintf("-l%s", libdirs[i]));
    }
}


int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char *inputs[] = {
        "graph.c"
    };
    Nob_Cmd cmd = {0};

    nob_cmd_append(&cmd, "cc", "-o", "run");
    cflags(&cmd);
    nob_da_append_many(&cmd, inputs, NOB_ARRAY_LEN(inputs));
    nob_cmd_append(&cmd, "-I/usr/local/include/graphviz");
    nob_cmd_append(&cmd, "-L/usr/local/lib");
    visualize(&cmd);
    if(nob_cmd_run_sync(cmd)){
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, "./run",">", "/tmp/gr.gv");
        nob_cmd_run_sync(cmd);
    }
    
    return 0;
}
