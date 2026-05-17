/* fast_sort.h
 *
 * Iterative introsort with 3-way partition and heapsort fallback.
 * Author: @forgehk
 */

#ifndef FAST_SORT_H
#define FAST_SORT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const char first_name[];
extern const char last_name[];

/* Sort the array `a` of length `n` in non-decreasing order. */
void fast_sort(int *a, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* FAST_SORT_H */
