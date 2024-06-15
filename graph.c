#include <gvc.h>

int main(int argc, char **argv)
{
    Agraph_t *g;
    Agnode_t *n, *m;
    Agedge_t *e;
    Agsym_t *a;
    GVC_t *gvc;
    /* set up a graphviz context */
    gvc = gvContext();
    /* parse command line args - minimally argv[0] sets layout engine */
    char *args[] = { "dot" };
    gvParseArgs(gvc, sizeof(args)/sizeof(*args), args);
    //gvParseArgs(gvc, 1, args);
    /* Create a simple digraph */
    g = agopen("g", Agdirected, NULL);
    n = agnode(g, "asdasdn\nsdsdsd", 1);
    m = agnode(g, "m", 1);
    e = agedge(g, n, m, 0, 1);
    /* Set an attribute - in this case one that affects the visible rendering */
    agsafeset(n, "color", "red", "");
    /* Compute a layout using layout engine from command line args */
    gvLayoutJobs(gvc, g);
    /* Write the graph according to -T and -o options */
    gvRenderJobs(gvc, g);
    /* Free layout data */
    gvFreeLayout(gvc, g);
    /* Free graph structures */
    agclose(g);
    /* close output file, free context, and return number of errors */
    gvFreeContext(gvc);
    return 0;
}
