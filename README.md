# HPC 2026 Homework 1: OpenMP

## Scripts

Run all experiments with:

```bash
./scripts/run_all.sh
```

Each script compiles the needed program, runs it, writes output to `results/*.txt`, and removes its executable.

## Task 1: BugReduction.c

There were several mistakes in the original program:

- `sum` was not initialized inside `dotprod`;
- `#pragma omp for` was used without returning the value to `main`;
- the result of `dotprod` was ignored.

The corrected version uses reduction:

```c
#pragma omp parallel for reduction(+ : sum)
```

Verification run:

```bash
OMP_NUM_THREADS=4 ./BugReduction
```

End of output:

```text
Sum = 328350.000000
```

The value is correct, because:
`0^2 + 1^2 + ... + 99^2 = 328350`

## Task 2: BugParFor.c

The original code opened a parallel region, but the ordinary loop was still run by each thread. I changed it to:

```c
#pragma omp parallel for schedule(static, chunk)
```

Each array element is calculated once, and every line prints which thread calculated it.

Example run:

```bash
OMP_NUM_THREADS=4 ./BugParFor
```

## Task 3: Pi.c

`Pi.c` calculates

```text
integral from 0 to 1 of 4 / (1 + x*x)
```

The loop iterations do not depend on each other, so the sum can be reduced in parallel. Time is measured with `omp_get_wtime()`.

Command pattern:

```bash
OMP_NUM_THREADS=<p> ./Pi 50000000
```

Measured results:

| Threads | Pi value | Time (secs) | Speedup |
|---:|---:|---:|---:|
| 1 | 3.1415926535895617 | 0.075261 | 1.00 |
| 2 | 3.1415926535900307 | 0.033847 | 2.22 |
| 4 | 3.1415926535897385 | 0.018124 | 4.15 |
| 8 | 3.1415926535897900 | 0.014777 | 5.09 |

The speedup for 2 and 4 threads is a little higher than the increase in thread count. This can happen because the run is short, so cache effects, CPU frequency changes, and usual measurement noise have visible influence.

## Task 4: Car.cpp

The program reads `car.ppm` and shifts the picture one column to the right. In each row the last pixel goes to the first position.

The shifting loop is parallelized by rows:

```cpp
#pragma omp parallel for
for (int row = 0; row < img.y; ++row)
```

Command pattern:

```bash
OMP_NUM_THREADS=4 ./Car <frames> <save_each>
```

The timing shows that saving many PPM files takes most of the time.

| Frames | Save each | Files saved | Time (secs) |
|---:|---:|---:|---:|
| 30 | 30 | 1 | 0.007221 |
| 30 | 15 | 2 | 0.010542 |
| 30 | 1 | 30 | 0.128928 |

## Task 5: Axisb.c

`Axisb.c` solves `Ax = b` by the Jacobi method. The matrix is generated inside the program:

- diagonal elements are `2*n`;
- all other elements are `1`.

This matrix is diagonally dominant. For checking the answer, `b` is made so that the correct solution is `x = 1`.

Parallel loops are used for:

- preparing `b`;
- calculating the next Jacobi vector;
- copying the new vector back;
- checking the maximum error.

Command pattern:

```bash
OMP_NUM_THREADS=<p> ./Axisb 1200 1000
```

Results:

| Threads | Iterations | Max error | Time (secs) | Speedup |
|---:|---:|---:|---:|---:|
| 1 | 29 | 1.818154293787e-09 | 0.087066 | 1.00 |
| 2 | 29 | 1.818154293787e-09 | 0.035555 | 2.45 |
| 4 | 29 | 1.818154293787e-09 | 0.020017 | 4.35 |
| 8 | 29 | 1.818154293787e-09 | 0.014027 | 6.21 |

## Task 6: LeastSquares.c

The program solves a linear regression problem for the model:

```text
y = a*x + b
```

The points are made close to

```text
y = 2*x + 5
```

with a small sine error added. The program computes these sums in parallel:

```text
sum(x), sum(y), sum(x*x), sum(x*y)
```

Then the normal equations give `a` and `b`.

Command pattern:

```bash
OMP_NUM_THREADS=<p> ./LeastSquares 5000000
```

Measured results:

| Threads | a | b | Time (secs) | Speedup |
|---:|---:|---:|---:|---:|
| 1 | 1.9999999972 | 5.0000000046 | 0.072104 | 1.00 |
| 2 | 1.9999999972 | 5.0000000046 | 0.027180 | 2.65 |
| 4 | 1.9999999972 | 5.0000000046 | 0.013486 | 5.35 |
| 8 | 1.9999999972 | 5.0000000046 | 0.007114 | 10.14 |

Here the measured speedup is also higher than ideal. I think this is not real perfect scalability, but an effect of short timings, cache behavior, CPU boost, and measurement noise. Repeating every run several times and averaging would give more stable numbers.
