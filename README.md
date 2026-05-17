# polyglot-sort-grand-prix

> Generic sorting implemented in **C, C++, and C#** — including a hand-rolled iterative introsort that beats `qsort` on adversarial inputs.

[![C](https://img.shields.io/badge/C-C99-blue.svg)]() [![C++](https://img.shields.io/badge/C++-17-blue.svg)]() [![C#](https://img.shields.io/badge/C%23-.NET%208-512BD4.svg)]() [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

---

## What this is

A study in how the same sorting problem is solved idiomatically across three very different languages, plus a custom high-performance sort.

An iterative introsort tuned to beat `qsort(3)` on duplicate-heavy and adversarial inputs, plus the same problem solved idiomatically in C++ and C#.

---

## The three implementations

| Language | Approach | Highlights |
|---|---|---|
| **C** | Custom iterative introsort | 3-way Dutch National Flag partition · median-of-3 pivot · binary insertion sort for small partitions · heapsort fallback on depth limit · dynamic stack (no recursion) |
| **C++** | `std::sort` + lambdas | Generic numeric sort · multi-key sort (age desc, name asc tie-break) · STL idioms |
| **C#** | `List<T>.Sort` + delegates | Same problem in .NET · culture-invariant string compare · `IComparer` style lambdas |

---

## The custom C introsort

The C implementation (`c/fast_sort.c`) is the highlight. Key design choices:

- **Iterative, not recursive.** Uses a dynamically-grown explicit stack of `Range` structs, so it doesn't blow the call stack on huge arrays.
- **3-way partition (Dutch National Flag).** Groups elements equal to the pivot together — collapsing duplicate runs in a single pass. This is the win on duplicate-heavy inputs.
- **Median-of-3 pivot.** Cheap defense against already-sorted and reverse-sorted adversarial inputs.
- **Binary insertion sort for small partitions** (cutoff: 32). Cache-friendly and avoids partition overhead at the tail.
- **Heapsort fallback at depth `2·log₂(n)`** — same trick `std::sort` uses to guarantee `O(n log n)` worst case.
- **Allocation-failure safe.** If the stack can't grow, the affected range falls back to heapsort in place. Never panics, never leaks.

```
fast_sort(int *a, size_t n)
  ├─ size < 32           → binary insertion sort
  ├─ depth == 0          → heapsort  (worst-case bound)
  └─ otherwise           → 3-way partition around median-of-3 pivot
                             └─ push both subranges to stack, larger first
```

---

## Build & run

### C
```bash
cd c
make
./sort_demo                # runs benchmark vs qsort on adversarial inputs
```

### C++
```bash
cd cpp
g++ -std=c++17 -O2 cpp_sort.cpp -o cpp_sort
./cpp_sort
```

### C#
```bash
cd csharp
dotnet run
```

---

## Why three languages?

The exercise: implement the same problem across paradigms. It's a good lens for spotting where languages help vs. get in the way:

- **C** forces you to think about memory, stack depth, and what the comparator actually *costs*. You can't hide behind a standard library.
- **C++** gives you the lambdas and STL to express the same sorts in one line — at the cost of compile-time complexity and template error messages.
- **C#** trades some of C++'s raw control for a much friendlier delegate model and a runtime that does the heavy lifting.

---

## License

[MIT](LICENSE) — use anything here however you like.

---

*Built by [@forgehk](https://github.com/forgehk) — [DarkForge AI](https://darkforgeai.com)*
