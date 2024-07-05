#include <stdio.h>

int main()
{
    FILE *gen = fopen("shared/result.h", "w");
    fprintf(gen, "#include \"../manual.h\"\n");
    fclose(gen);
    return 0;
}
