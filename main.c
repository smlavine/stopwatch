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
	int opt;
	bool clearfile = false;
	FILE *out = stdout;
	char *filename = NULL;
	time_t start, now;
	double diff;
	unsigned long long seconds, h, m, s;

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
		filename = argv[0];
		if ((out = fopen(filename, "w")) == NULL) {
			fprintf(stderr, "Error opening '%s': %s\n",
					filename, strerror(errno));
			return EXIT_FAILURE;
		}
		break;
	default:
		fputs(HELP_STRING, stdout);
		return EXIT_FAILURE;
		break;
	}

	/* Set filename if there was no FILE argument. In case of an error,
	 * it will be printed, and NULL can't be printed.
	 */
	if (filename == NULL)
		filename = "/dev/stdout";

	if ((start = time(NULL)) == (time_t)-1)
		return EXIT_FAILURE;

	for (;;) {
		if ((now = time(NULL)) == (time_t)-1)
			return EXIT_FAILURE;

		diff = difftime(now, start);
		if (isnan(diff) || diff < 0 || MAX_PRECISE_DOUBLE < diff)
			return EXIT_FAILURE;

		seconds = diff;
		h = seconds / 3600;
		seconds %= 3600;
		m = seconds / 60;
		seconds %= 60;
		s = seconds;

		if (clearfile && (out = freopen(filename, "w", out)) == NULL) {
			fprintf(stderr, "Error reopening '%s': %s\n",
					filename, strerror(errno));
			return EXIT_FAILURE;
		}

		if (fprintf(out, "\r%02llu:%02llu:%02llu", h, m, s) < 0) {
			fprintf(stderr, "Error writing to '%s': %s\n",
					filename, strerror(errno));
			return EXIT_FAILURE;
		}

		fflush(out);

		sleep(1);
	}
}
