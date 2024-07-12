#define NOB_IMPLEMENTATION
#include "nob.h"
#include "state.h"

#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

void cflags(Nob_Cmd *cmd)
{
    nob_cmd_append(cmd, "-Wall", "-Wextra", "-ggdb");
}

typedef STATE *process(STATE *field, char input);


#define LOGIC_PATH "shared/logic.so"
void *handle;
process *hotreload() 
{
    if(handle != NULL)
        dlclose(handle);
    while(1) 
    {
        Nob_Cmd generics_run = {0};
        nob_cmd_append(&generics_run, "./gen");
        if (!nob_cmd_run_sync(generics_run)) 
        {
            printf("Generics build failed ENTER to redo.\n");
            while(getchar()!='\n'){}
            continue;
        }

        const char *inputs[] = {
            "logic.c"
        };
        Nob_Cmd logic_cmd = {0};
        nob_cmd_append(&logic_cmd, "cc", "-shared", "-o", LOGIC_PATH);
        cflags(&logic_cmd);
        nob_da_append_many(&logic_cmd, inputs, NOB_ARRAY_LEN(inputs));
        if (!nob_cmd_run_sync(logic_cmd))
        {
            printf("Logic compilation faild ENTER to redo.\n");
            while(getchar()!='\n'){}
            continue;
        }
        handle = dlopen(LOGIC_PATH, RTLD_NOW);
        return dlsym(handle, "process");
    }
}

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    Nob_Cmd generics_build = {0};
    nob_cmd_append(&generics_build, "cc", "generics.c", "-o", "gen");
    nob_cmd_run_sync(generics_build);

    process *logic = hotreload();
    
    STATE *field = calloc(1, sizeof(STATE));
    field->figure = 2;
    while(1){
       char inp = getchar();
       while(getchar()!='\n') {}
       if(inp=='r')
            logic = hotreload();
       else
            field = logic(field, inp);
    }
    
    return 0;
}
