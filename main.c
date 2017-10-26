#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void printplan(FILE* fh);
char* plan_name(void);

int main(int argc, char* argv[])
{
    char* filename = plan_name();
    FILE* fh = fopen(filename, "r");
    if (fh == NULL) {
        fprintf(stderr, "%s\n", strerror(errno));
    }
    printplan(fh);
    fclose(fh);

    return EXIT_SUCCESS;
}

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

char* plan_name(void) {
    char* home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "Environment variable HOME is not defined\n");
        return NULL;
    }

    char* filename;
    if (asprintf(&filename, "%s/.plan", home) == -1) {
        fprintf(stderr, "Unable to construct path to .plan file\n");
        return NULL;
    }

    return filename;
}
