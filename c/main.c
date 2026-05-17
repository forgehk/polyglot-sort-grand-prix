/* main.c
 *
 * Driver / benchmark for fast_sort.
 * Compares fast_sort to qsort(3) on a few adversarial input patterns.
 *
 * Author: @forgehk
 */

#include "fast_sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

static int *make_random(size_t n) {
    int *a = malloc(n * sizeof(int));
    for (size_t i = 0; i < n; i++) a[i] = rand();
    return a;
}

static int *make_sorted(size_t n) {
    int *a = malloc(n * sizeof(int));
    for (size_t i = 0; i < n; i++) a[i] = (int)i;
    return a;
}

static int *make_reverse(size_t n) {
    int *a = malloc(n * sizeof(int));
    for (size_t i = 0; i < n; i++) a[i] = (int)(n - i);
    return a;
}

static int *make_duplicates(size_t n) {
    int *a = malloc(n * sizeof(int));
    for (size_t i = 0; i < n; i++) a[i] = rand() % 10;
    return a;
}

static int is_sorted(const int *a, size_t n) {
    for (size_t i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) return 0;
    }
    return 1;
}

static double time_sort(void (*sorter)(int *, size_t), int *a, size_t n) {
    clock_t t0 = clock();
    sorter(a, n);
    clock_t t1 = clock();
    return (double)(t1 - t0) / CLOCKS_PER_SEC;
}

static void qsort_wrapper(int *a, size_t n) {
    qsort(a, n, sizeof(int), cmp_int);
}

static void run_case(const char *name, int *(*maker)(size_t), size_t n) {
    int *a = maker(n);
    int *b = malloc(n * sizeof(int));
    memcpy(b, a, n * sizeof(int));

    double t_fast = time_sort(fast_sort, a, n);
    double t_qsrt = time_sort(qsort_wrapper, b, n);

    printf("%-20s n=%zu  fast_sort=%.4fs  qsort=%.4fs  correct=%s\n",
           name, n, t_fast, t_qsrt,
           (is_sorted(a, n) && is_sorted(b, n)) ? "yes" : "NO");

    free(a);
    free(b);
}

int main(void) {
    srand(42);
    const size_t N = 1000000;

    printf("Author: %s %s\n\n", first_name, last_name);

    run_case("random",       make_random,     N);
    run_case("already sorted", make_sorted,   N);
    run_case("reverse sorted", make_reverse,  N);
    run_case("many duplicates", make_duplicates, N);

    return 0;
}
