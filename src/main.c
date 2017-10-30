#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void print_help()
{
    printf("HELP\n");
}

void print_version()
{
    printf("VERSION\n");
}

enum state {
    SKIP_ALL,
    SKIP_SPACE,
    PRINT,
    BEGIN_LINE
};

void print_plan(FILE* fh)
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

char* plan_name(void)
{
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

int are_working_hours(time_t* time)
{
    struct tm* t = localtime(time);
    if (t->tm_wday == 0 || t->tm_wday == 6) {
        // Saturday or Sunday
        return 0;
    } else if (9 <= t->tm_hour && t->tm_hour < 18) {
        // Weekday, between 9:00 and 18:00
        return 1;
    } else {
        // Weekday, outside working hours
        return 0;
    }
}

int main(int argc, char* argv[])
{
    int c;
    while ((c = getopt(argc, argv, "hv")) != -1) {
        switch (c) {
        case 'v':
            print_version();
            return EXIT_SUCCESS;
        case '?':
        case 'h':
        default:
            print_help();
            return EXIT_SUCCESS;
        }
    }
    argc -= optind;
    argv += optind;

    time_t now = time(NULL);
    if (!are_working_hours(&now)) {
        return EXIT_SUCCESS;
    }

    char* filename = plan_name();
    FILE* fh = fopen(filename, "r");
    if (fh == NULL) {
        fprintf(stderr, "%s\n", strerror(errno));
    }
    print_plan(fh);
    fclose(fh);

    return EXIT_SUCCESS;
}
