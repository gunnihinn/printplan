#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "help.h"

void print_help()
{
    fwrite(src_help_txt, sizeof(char), src_help_txt_len, stdout);
}

int ignore(char* line)
{
    return *line == '\0' || !isalpha(*line);
}

void print_plan(FILE* fh)
{
    // TODO: Deal with lines longer than this?
    unsigned int bufsize = 2 << 10;
    char* buffer = malloc(sizeof(char) * bufsize);

    while (!feof(fh)) {
        fgets(buffer, bufsize, fh);
        if (!ignore(buffer)) {
            printf("%s", buffer);
        }
    }

    free(buffer);
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
    int force_print = 0;

    int c;
    while ((c = getopt(argc, argv, "fh")) != -1) {
        switch (c) {
        case 'f':
            force_print = 1;
            break;
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
    if (!force_print && !are_working_hours(&now)) {
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
