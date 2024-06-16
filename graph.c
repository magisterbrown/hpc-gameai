#include <gvc.h>

int main(void)
{
    Agraph_t *g;
    Agnode_t *n, *m;
    //Agsym_t *a;
    GVC_t *gvc;
    /* set up a graphviz context */
    gvc = gvContext();
    ///* parse command line args - minimally argv[0] sets layout engine */
    char *args[] = {"dot", "-ograph.svg", "-Tsvg" , ""};
    gvParseArgs(gvc, 3, args);
    //gvParseArgs(gvc, 1, args);
    ///* Create a simple digraph */
    g = agopen("g", Agdirected, NULL);
    n = agnode(g, "fuuu", 1);
    m = agnode(g, "m", 1);
    agedge(g, n, m, 0, 1);
    ///* Set an attribute - in this case one that affects the visible rendering */
    //agsafeset(n, "color", "red", "");
    ///* Compute a layout using layout engine from command line args */
    gvLayoutJobs(gvc, g);
    /////* Write the graph according to -T and -o options */
    gvRenderJobs(gvc, g);
    /////* Free layout data */
    gvFreeLayout(gvc, g);
    /////* Free graph structures */
    agclose(g);
    /* close output file, free context, and return number of errors */
    gvFreeContext(gvc);
    return 0;
}
