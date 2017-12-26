#include <unistd.h> // sleep()
#include <tgmath.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifndef __STDC_IEC_559__
#error "unsupported double"
#endif
static_assert(sizeof(double) == 8, "double must have double precision");
#define MAX_PRECISE_DOUBLE ((double)(1ULL << 52))

int main(void) {
    time_t start = time(NULL);
    if (start == (time_t)-1) return EXIT_FAILURE;
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

        (void)printf("\r%02llu:%02llu:%02llu", h, m, s);
        (void)fflush(stdout);

        (void)sleep(1);
    }
}
