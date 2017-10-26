#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

enum state {
    SKIP_ALL,
    SKIP_SPACE,
    PRINT,
    BEGIN_LINE
};

void printplan(FILE* fh)
{
    char c;
    enum state state = BEGIN_LINE;
    while ((c = getc(fh)) != EOF) {
        switch (state) {
        case PRINT:
            if (c == '\n') {
                state = BEGIN_LINE;
            }
            printf("%c", c);
            break;
        case SKIP_SPACE:
            if (!isspace(c)) {
                state = PRINT;
                printf("%c", c);
            }
            break;
        case BEGIN_LINE:
            if (c == '.') {
                state = SKIP_SPACE;
            } else if (c != '\n') {
                state = SKIP_ALL;
            }
            break;
        case SKIP_ALL:
            if (c == '\n') {
                state = BEGIN_LINE;
            }
            break;
        default:
            break;
        }
    }
}

int main(int argc, char* argv[])
{
    char* home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "Environment variable HOME is not defined\n");
        return EXIT_FAILURE;
    }

    char* filename;
    if (asprintf(&filename, "%s/.plan", home) == -1) {
        fprintf(stderr, "Unable to construct path to .plan file\n");
        return EXIT_FAILURE;
    }

    FILE* fh = fopen(filename, "r");
    printplan(fh);

    return EXIT_SUCCESS;
}
