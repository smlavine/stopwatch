#include <assert.h>
#include <errno.h>
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

#define OPTSTRING "ch"

static const char *HELP_STRING =
"usage: stopwatch [-" OPTSTRING "] [FILE]\n"
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
	time_t start;
	FILE *out = stdout;
	char *filename = NULL;
	bool clearfile = false;

	if ((start = time(NULL)) == (time_t)-1)
		return EXIT_FAILURE;

	while ((opt = getopt(argc, argv, OPTSTRING)) != -1) {
		switch (opt) {
		case 'c':
			clearfile = true;
			break;
		case 'h':
			fputs(HELP_STRING, stdout);
			return EXIT_SUCCESS;
			break;
		case ':':
		case '?':
			fputs(HELP_STRING, stdout);
			return EXIT_FAILURE;
			break;
		}
	}
	argv += optind;
	argc -= optind;

	switch (argc) {
	case 0:
		break;
	case 1:
		if ((out = fopen(argv[0], "w")) == NULL) {
			fprintf(stderr, "Error opening '%s': %s\n",
					argv[0], strerror(errno));
			return EXIT_FAILURE;
		}
		break;
	default:
		fputs(HELP_STRING, stdout);
		return EXIT_FAILURE;
		break;
	}

	for (;;) {
		time_t now;
		if ((now = time(NULL)) == (time_t)-1)
			return EXIT_FAILURE;

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
