// fast_sort.c
//
// Iterative introsort tuned to beat qsort on adversarial inputs
// @forgehk — iterative introsort with 3-way partition + heapsort fallback
//
// Notes:
// - Uses only stdlib.h (malloc/realloc/free) and fast_sort.h.
// - Iterative (dynamic stack) to avoid recursion depth issues on huge arrays.
// - 3-way partition (Dutch National Flag) handles many duplicates efficiently.
// - Binary insertion sort for small partitions, heapsort fallback on depth limit.

#include "fast_sort.h"
#include <stdlib.h>

const char first_name[] = "@forgehk";
const char last_name[]  = "forgehk";

#define SWAP(a,b) do { int _t=(a); (a)=(b); (b)=_t; } while(0)

static inline size_t ilog2_size(size_t n){
    size_t k = 0;
    while(n > 1){ n >>= 1; ++k; }
    return k;
}

static inline void binary_insertion_sort(int *a, size_t lo, size_t hi){
    for(size_t i = lo + 1; i <= hi; ++i){
        int x = a[i];
        size_t l = lo, r = i;
        // first position >= x
        while(l < r){
            size_t m = l + ((r - l) >> 1);
            if(a[m] <= x) l = m + 1; else r = m;
        }
        for(size_t j = i; j > l; --j) a[j] = a[j - 1];
        a[l] = x;
    }
}

static inline size_t med3_idx(const int *a, size_t i, size_t j, size_t k){
    int x = a[i], y = a[j], z = a[k];
    return (x < y) ? ((y < z) ? j : (x < z ? k : i))
                   : ((y > z) ? j : (x < z ? i : k));
}

static inline void sift_down(int *a, size_t base, size_t n, size_t i){
    // max-heap sift
    for(;;){
        size_t l = (i << 1) + 1;
        if(l >= n) break;
        size_t r = l + 1;
        size_t m = (r < n && a[base + r] > a[base + l]) ? r : l;
        if(a[base + m] <= a[base + i]) break;
        SWAP(a[base + i], a[base + m]);
        i = m;
    }
}

static inline void heapsort(int *a, size_t lo, size_t hi){
    size_t n = hi - lo + 1;
    if(n < 2) return;
    // build heap
    if(n >= 2){
        for(size_t i = (n - 2) >> 1; /*loop*/ ; ){
            sift_down(a, lo, n, i);
            if(i == 0) break;
            --i;
        }
    }
    // sortdown
    for(size_t end = n - 1; end > 0; --end){
        SWAP(a[lo], a[lo + end]);
        sift_down(a, lo, end, 0);
    }
}

// 3-way (Dutch National Flag) partition around pivot p
// returns [lt_end, gt_begin): range of elements equal to p
static inline void partition_3way(int *a, size_t lo, size_t hi, int p,
                                  size_t *lt_end, size_t *gt_begin)
{
    size_t lt = lo;
    size_t i  = lo;
    size_t gt = hi;

    while(i <= gt){
        int v = a[i];
        if(v < p){
            SWAP(a[lt], a[i]);
            ++lt; ++i;
        }else if(v > p){
            SWAP(a[i], a[gt]);
            if(gt == lo){ // can't go below lo without underflow
                // After swap, the element at 'i' is new and must be rechecked.
                // Shrink gt to lo; loop will end soon when i > gt.
                break;
            }
            --gt;
        }else{
            ++i;
        }
    }
    // If we broke early on gt==lo and still i<=gt, we need to finish the scan safely.
    while(i <= gt){
        int v = a[i];
        if(v < p){
            SWAP(a[lt], a[i]);
            ++lt; ++i;
        }else if(v > p){
            // gt == lo here, so v>p at i cannot be moved further right; swap with itself.
            // Move i forward to ensure progress.
            ++i;
        }else{
            ++i;
        }
    }

    *lt_end   = lt;
    *gt_begin = gt + 1;
}

typedef struct { size_t lo, hi, depth; } Range;

static int push_range(Range **stack, size_t *sp, size_t *cap,
                      size_t lo, size_t hi, size_t depth)
{
    if(lo >= hi) return 1; // nothing to do, but not a failure
    if(*sp == *cap){
        size_t ncap = (*cap) ? (*cap) << 1 : 64;
        Range *nst = (Range*)realloc(*stack, ncap * sizeof(Range));
        if(!nst) return 0;
        *stack = nst; *cap = ncap;
    }
    (*stack)[(*sp)++] = (Range){lo, hi, depth};
    return 1;
}

void fast_sort(int *a, size_t n){
    if(n < 2) return;

    const size_t SMALL = 32;                       // insertion cutoff
    const size_t MAX_DEPTH = (ilog2_size(n) << 1); // ~2*floor(log2 n)

    size_t cap = 64, sp = 0;
    Range *stack = (Range*)malloc(cap * sizeof(Range));
    if(!stack){
        // fallback: single heapsort if allocation fails
        heapsort(a, 0, n - 1);
        return;
    }

    stack[sp++] = (Range){0, n - 1, MAX_DEPTH};

    while(sp){
        Range r = stack[--sp];
        size_t lo = r.lo, hi = r.hi, depth = r.depth;

        if(hi <= lo) continue;

        size_t len = hi - lo + 1;
        if(len <= SMALL){
            binary_insertion_sort(a, lo, hi);
            continue;
        }

        if(depth == 0){
            heapsort(a, lo, hi);
            continue;
        }

        // median-of-3 pivot
        size_t mid = lo + ((hi - lo) >> 1);
        size_t m   = med3_idx(a, lo, mid, hi);
        int p      = a[m];

        // 3-way partition
        size_t lt_end, gt_begin;
        partition_3way(a, lo, hi, p, &lt_end, &gt_begin);

        size_t left_sz  = (lt_end > lo) ? (lt_end - lo) : 0;
        size_t right_sz = (gt_begin <= hi) ? (hi - gt_begin + 1) : 0;

        // push larger first to keep stack shallow; if push fails, heapsort the range
        if(left_sz >= right_sz){
            if(right_sz > 1){
                if(!push_range(&stack, &sp, &cap, gt_begin, hi, depth - 1)){
                    heapsort(a, gt_begin, hi);
                }
            }
            if(left_sz > 1){
                if(!push_range(&stack, &sp, &cap, lo, lt_end - 1, depth - 1)){
                    heapsort(a, lo, lt_end - 1);
                }
            }
        }else{
            if(left_sz > 1){
                if(!push_range(&stack, &sp, &cap, lo, lt_end - 1, depth - 1)){
                    heapsort(a, lo, lt_end - 1);
                }
            }
            if(right_sz > 1){
                if(!push_range(&stack, &sp, &cap, gt_begin, hi, depth - 1)){
                    heapsort(a, gt_begin, hi);
                }
            }
        }
    }

    free(stack);
}
