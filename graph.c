#include <gvc.h>
#include "node.h"
#include <stdio.h>

#define ARENA_IMPLEMENTATION
#include "external/arena.h"
void record(Node *parrent, Agnode_t *pnode, Agraph_t *graph) {
    for(int i=0;i<FIELD_X;i++){
        if(parrent->children[i] != NULL){
            Agnode_t *nnode;
            char tag[10];
            sprintf(tag, "%d", i);
            nnode = agnode(graph, tag, 1);
            agedge(graph, pnode, nnode, 0, 1);
        }
    }
}
int main(void) {
    static Arena default_arena = {0}; 
    Node *root = arena_alloc(&default_arena, sizeof(Node));
    search(root, &default_arena);

    Agraph_t *g;
    Agnode_t *rnode;
    GVC_t *gvc;
    gvc = gvContext();
    char *args[] = {"dot", "-ograph.svg", "-Tsvg" , ""};
    gvParseArgs(gvc, 3, args);
    g = agopen("search", Agdirected, NULL);
    rnode = agnode(g, "root", 1);
    agsafeset(rnode, "color", "red", "");
   
    record(root, rnode, g);

    gvLayoutJobs(gvc, g);
    gvRenderJobs(gvc, g);
    gvFreeLayout(gvc, g);
    agclose(g);
    arena_free(&default_arena);
    return 0;
}

//int notmain(void)
//{
//    Agnode_t *n, *m;
//    //Agsym_t *a;
//    GVC_t *gvc;
//    /* set up a graphviz context */
//    gvc = gvContext();
//    ///* parse command line args - minimally argv[0] sets layout engine */
//    char *args[] = {"dot", "-ograph.svg", "-Tsvg" , ""};
//    gvParseArgs(gvc, 3, args);
//    //gvParseArgs(gvc, 1, args);
//    ///* Create a simple digraph */
//    g = agopen("g", Agdirected, NULL);
//    n = agnode(g, "fuuu", 1);
//    m = agnode(g, "m", 1);
//    agedge(g, n, m, 0, 1);
//    ///* Set an attribute - in this case one that affects the visible rendering */
//    //agsafeset(n, "color", "red", "");
//    ///* Compute a layout using layout engine from command line args */
//    gvLayoutJobs(gvc, g);
//    /////* Write the graph according to -T and -o options */
//    gvRenderJobs(gvc, g);
//    /////* Free layout data */
//    gvFreeLayout(gvc, g);
//    /////* Free graph structures */
//    agclose(g);
//    /* close output file, free context, and return number of errors */
//    gvFreeContext(gvc);
//    return 0;
//}
