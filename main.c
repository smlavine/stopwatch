#include <unistd.h> // sleep()
#include <tgmath.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#ifndef __STDC_IEC_559__
#error "unsupported double"
#endif
static_assert(sizeof(double) == 8, "double must have double precision");
#define MAX_PRECISE_DOUBLE ((double)(1ULL << 52))

static const char *HELP_STRING =
"usage: stopwatch [-ch] [-t FILE]\n"
"Options:\n"
"-h  Prints this help message.\n"
"-t  Prints output to FILE\n"
"-c  If possible, clears FILE contents\n"
"    before every write.\n"
"Written by Mark Weston and improved by\n"
"Sebastian LaVine. This program is Free Software,\n"
"licensed under the ISC license. See the file\n"
"LICENSE for details.";

/**
 * Original program written by Mark Weston. Extra options added
 * by Sebastian LaVine. See LICENSE for copyright details.
 */
int main(int argc, char *argv[]) {
    time_t start = time(NULL);
    if (start == (time_t)-1) return EXIT_FAILURE;

    /* File output added by Sebastian LaVine <seblavine@outlook.com> */
    FILE *out = stdout;
    bool clearfile = false;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; j < strlen(argv[i]); j++) {
                switch (argv[i][j]) {
                case 't':
                    if ((out = fopen("stopwatch.txt", "w")) == NULL) {
                        fputs("Error opening log file\n", stderr);
                        return EXIT_FAILURE;
                    }
                    break;
                case 'c':
                    clearfile = true;
                    break;
                case '-':
                    i = argc; /* end of arguments */
                    break;
                case 'h':
                    puts(HELP_STRING);
                    return EXIT_SUCCESS;
                    break;
                }
            }
        }
    }
    
    if (clearfile && out == stdout)
        clearfile = false;

    while (1) {
        time_t now = time(NULL);
        if (now == (time_t)-1) return EXIT_FAILURE;

        double diff = difftime(now, start);
        if (isnan(diff) || diff < 0) return EXIT_FAILURE;
        if (diff > MAX_PRECISE_DOUBLE) return EXIT_FAILURE;

        unsigned long long seconds = diff;
        unsigned long long h = seconds / 3600;
        seconds %= 3600;
        unsigned long long m = seconds / 60;
        seconds %= 60;
        unsigned long long s = seconds;
        
        if (clearfile) {
            fclose(out);
            out = fopen("stopwatch.txt", "w");
        }
        (void)fprintf(out, "\r%02llu:%02llu:%02llu", h, m, s);
        (void)fflush(out);

        (void)sleep(1);
    }
}
