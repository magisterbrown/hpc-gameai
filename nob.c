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
process *hotreload(Nob_Cmd *cmd) 
{
    if(handle != NULL)
        dlclose(handle);

    while(!nob_cmd_run_sync(*cmd)){
        printf("Logic compilation faild ENTER to redo.\n");
        while(getchar()!='\n'){}
    }
    handle = dlopen(LOGIC_PATH, RTLD_NOW);
    return dlsym(handle, "process");

}

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char *inputs[] = {
        "logic.c"
    };
    Nob_Cmd logic_cmd = {0};
    char* logic_path = "shared/logic.so";
    nob_cmd_append(&logic_cmd, "cc", "-shared", "-o", LOGIC_PATH);
    cflags(&logic_cmd);
    nob_da_append_many(&logic_cmd, inputs, NOB_ARRAY_LEN(inputs));
    
    process *logic = hotreload(&logic_cmd);
    
    STATE *field = calloc(1, sizeof(STATE));
    field->figure = 1;
    while(1){
       char inp = getchar();
       while(getchar()!='\n') {}
       if(inp=='r')
            logic = hotreload(&logic_cmd);
       else
            field = logic(field, inp);
    }
    
    return 0;
}
