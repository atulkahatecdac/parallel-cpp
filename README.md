# Parallel Programming in C++ — Course Programs

Small, self-contained C++ programs matching the 6-module course deck
(`Parallel_Programming_Cpp.pptx`). Every program is short (under ~100 lines)
and focuses on exactly one idea. The core 23 examples are numbered
sequentially (01-23) in the order they're meant to be studied, across both
folders and files; a few numbers also have a lettered variant (e.g. `01b`,
`11b`) added later to explore a related idea, plus three additional
examples (24-26) extending Module 6.

## Folder layout and numbering

- `00_intro_examples/` (01-07) — one tiny "hello world" per new concept
  (lambdas, templates, OpenMP, Pthreads, mutex, atomic). Read these before
  the matching module's real program.
  - `01_lambda_hello.cpp`
  - `01b_lambda_parallel_stl.cpp` — the same lambda syntax passed to a
    C++17 parallel STL algorithm (`std::for_each` with
    `std::execution::par`) instead of OpenMP/Pthreads. Requires linking
    against Intel TBB (see Requirements below).
  - `02_template_hello.cpp`
  - `02b_complexity_classes.cpp` — the six Big-O classes (O(1) through
    O(2^n)) from the "Complexity and Classification" slide, shown two
    ways: an operation-count table (comparisons, recursive calls) that
    reproduces the slide's numbers exactly, and a wall-clock timing table
    (each operation repeated enough times to be measurable) showing the
    same growth rates in real seconds.
  - `03_openmp_hello.cpp`
  - `04_openmp_parallel_for.cpp`
  - `05_pthreads_hello.cpp`
  - `06_mutex_hello.cpp`
  - `07_atomic_hello.cpp`
- `module1_fundamentals/` (08) — lambda basics applied to an array sum.
  - `08_lambda_array_sum.cpp`
- `module2_concepts/` (09-14) — serial vs. parallel Fibonacci, array
  increment, and Pthreads vs. OpenMP for the same sum task.
  - `09_fibonacci_serial.cpp`
  - `10_fibonacci_openmp.cpp`
  - `10b_fork_join_without_join.cpp` / `10c_fork_join_with_join.cpp` —
    the fork-join pattern itself, isolated from any real workload: fork 4
    branches, then either skip the join (crashes - `std::terminate()` is
    called when a still-joinable `std::thread` is destroyed) or join
    properly (finishes cleanly).
  - `11_array_increment_serial.cpp`
  - `12_array_increment_openmp.cpp`
  - `11b_array_sqrt_serial.cpp` / `12b_array_sqrt_openmp.cpp` — same
    increment pattern, but with compute-heavy (repeated `sqrt`) work per
    element instead of `+1`, so the loop is compute-bound rather than
    memory-bound and the parallel version actually shows a speedup.
  - `13_sum_pthreads.cpp`
  - `14_sum_openmp.cpp` — sums an array with a plain
    `#pragma omp parallel for`, no `reduction` clause, so threads race on
    the shared `total`. Run it a few times: the result is wrong and
    varies (lost updates).
  - `14b_sum_openmp_reduction.cpp` — the fix: adds
    `reduction(+:total)` so each thread gets a private copy of `total`
    and OpenMP combines them at the end. Correct every time.
- `module3_experimental/` (15) — speedup/efficiency measured across
  1/2/4/8 threads, printed as a table.
  - `15_speedup_table.cpp`
- `module4_granularity/` (16) — parallel merge sort with an editable
  `THRESHOLD` constant to explore granularity.
  - `16_mergesort_threshold.cpp`
- `module5_algorithms/` (17-20) — reduction, scan, parallel search,
  parallel sort.
  - `17_reduction_sum.cpp`
  - `18_scan_prefix_sum.cpp`
  - `19_parallel_search.cpp`
  - `20_parallel_sort.cpp`
- `module6_race_conditions/` (21-26) — the same shared-counter program in
  three versions: unsynchronized (buggy), mutex-protected, atomic.
  - `21_counter_unsynchronized.cpp`
  - `22_counter_mutex.cpp`
  - `23_counter_atomic.cpp`
  - `24_bank_account_unsynchronized.cpp` / `25_bank_account_mutex.cpp` /
    `26_bank_account_ownership.cpp` — the same race on a more relatable
    shared resource (a bank balance), fixed two different ways: `25`
    synchronizes access to the still-shared balance with a mutex (like
    `22`); `26` instead redesigns data ownership so each thread only ever
    touches its own private total, with no shared variable - and
    therefore no lock - during the parallel work at all.

## Requirements

- A C++17 compiler with OpenMP support: GCC's `g++` (via MinGW-w64 on
  Windows, or natively on Linux/macOS). **Must be GCC-compatible** —
  Visual Studio's MSVC (`cl.exe`) will not work, since two of the intro
  examples use the POSIX `<pthread.h>` API directly, which MSVC doesn't
  provide.
- A `make`-compatible build tool.
- **Intel TBB** (`libtbb-dev` on Debian/Ubuntu), needed only for
  `01b_lambda_parallel_stl.cpp`. GCC's libstdc++ implementation of
  `std::execution::par` uses TBB as its threading backend — without it,
  that one program still compiles and runs, but silently falls back to
  sequential execution instead of an error.

### Installing on Windows

Install **MinGW-w64**, which provides both `g++` (with OpenMP built in)
and a `make` tool in one package:

1. Install [MSYS2](https://www.msys2.org/) (recommended — easiest to keep
   updated).
2. Open the "MSYS2 UCRT64" terminal from the Start menu and run:
   ```
   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make
   ```
3. Add MSYS2's `ucrt64\bin` folder (e.g.
   `C:\msys64\ucrt64\bin`) to your Windows PATH so `g++` and `make` are
   available from any terminal (PowerShell, Git Bash, cmd).

**Important naming note:** depending on how MinGW-w64 was installed, the
build tool may be called `make` **or** `mingw32-make` — some MinGW-w64
distributions (e.g. the standalone installer used to build this course)
only provide `mingw32-make.exe`, not `make.exe`. Check which one you have:
```
g++ --version
make --version          # try this first
mingw32-make --version  # if the above isn't found, try this instead
```
If only `mingw32-make` is found, either run that name directly (e.g.
`mingw32-make all`) everywhere this README says `make`, or create a copy
named `make.exe` next to it so plain `make` works too.

### Installing on Linux (Debian/Ubuntu example)

```
sudo apt update
sudo apt install build-essential libtbb-dev
```
`build-essential` includes `g++`, `make`, and OpenMP support
(`libgomp`) already — no extra OpenMP package is needed. `libtbb-dev` is
only needed for `01b_lambda_parallel_stl.cpp` (see Requirements above).

### Installing on macOS

Apple's default `clang++` (from Xcode Command Line Tools) does **not**
support OpenMP out of the box. Install real GCC via
[Homebrew](https://brew.sh/) instead:
```
xcode-select --install    # provides make and basic build tools
brew install gcc
```
Homebrew installs GCC under a versioned name (e.g. `g++-14`, not `g++`,
to avoid clashing with Apple's clang). Either invoke it by that name, or
update the Makefile's `CXX = g++` line to match (e.g. `CXX = g++-14`).

### Verifying the toolchain works

Run these before building the course:
```
g++ --version      # should print a GCC/MinGW version, not "clang"
make --version      # or mingw32-make --version on some Windows setups
```
Then confirm OpenMP itself is wired up correctly:
```
echo '#include <omp.h>
#include <cstdio>
int main(){
    #pragma omp parallel
    printf("thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads());
}' > /tmp/omp_check.cpp
g++ -fopenmp /tmp/omp_check.cpp -o /tmp/omp_check && /tmp/omp_check
```
If this prints one line per available CPU thread, OpenMP is working and
the rest of the course will build correctly.

## Building

```
make                          # builds every program into bin/
make module2                  # builds only Module 2's programs
make bin/10_fibonacci_openmp   # builds just one program
make clean                    # removes bin/
```

## Running

Each built program is a plain executable in `bin/`, numbered the same as
its source file, e.g.:

```
./bin/09_fibonacci_serial
./bin/10_fibonacci_openmp
./bin/21_counter_unsynchronized   # run a few times - result varies!
./bin/22_counter_mutex            # always correct
./bin/23_counter_atomic           # always correct
```

## Suggested order

Simply work through 01 to 26 in numeric order:

1. Run the matching `00_intro_examples/` program (01-07) for a new
   concept first.
2. Then run the serial version of a module's program, then the parallel
   version, and compare output/timing.
3. For Module 6 (21-26), run `21_counter_unsynchronized` several times to
   see the race condition before moving to the fixed versions, then do
   the same with `24_bank_account_unsynchronized` before its two fixes
   (`25`, `26`) - notice they fix the same race in two different ways.

## Note on `21_counter_unsynchronized` and `24_bank_account_unsynchronized`

Both programs are built with `-O0` (no optimization) in the Makefile, on
purpose. At `-O2`, the compiler can legally prove that "increment a
variable N times" is equivalent to "add N once" and collapse the whole
loop into a single operation per thread. That removes the very race
condition the demo is meant to show. Running it at `-O0` keeps the loop
as real repeated read-modify-write operations, so the race reliably
shows up as a wrong total.
