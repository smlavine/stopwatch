#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>
#include <time.h>
#include <unistd.h>

#ifndef __STDC_IEC_559__
#error "unsupported double"
#endif
static_assert(sizeof(double) == 8, "double must have double precision");
#define MAX_PRECISE_DOUBLE ((double)(1ULL << 52))

static const char *HELP_STRING =
"usage: stopwatch [-ch] [FILE]\n"
"Options:\n"
"-h    Prints this help message.\n"
"-c    If possible, clears FILE contents before every write.\n"
"FILE: Path to output file. If not given, defaults to stdout.\n"
"Written by Mark Weston and improved by\n"
"Sebastian LaVine. This program is Free Software,\n"
"licensed under the ISC license. See the file\n"
"LICENSE for details.\n";

int
main(int argc, char *argv[])
{
	time_t start = time(NULL);
	FILE *out = stdout;
	char *filename = NULL;
	bool clearfile = false;

	if (start == (time_t)-1) return EXIT_FAILURE;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			for (int j = 1; j < strlen(argv[i]); j++) {
				switch (argv[i][j]) {
				case 'c':
					clearfile = true;
					break;
				case 'h':
					fputs(HELP_STRING, stdout);
					return EXIT_SUCCESS;
					break;
				case '-':
					i = argc; /* end of arguments */
					break;
				}
			}
		} else { /* Not an option; FILE argument */
			if (filename != NULL) { /* If another FILE was given earlier, */
				fclose(out);		/* close file before opening another. */
			}
			filename = argv[i];
			if ((out = fopen(filename, "w")) == NULL) {
				fputs("Error opening log file\n", stderr);
				return EXIT_FAILURE;
			}
		}
	}

	for (;;) {
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

		fprintf(out, "\r%02llu:%02llu:%02llu", h, m, s);
		fflush(out);

		sleep(1);
	}
}
