#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    long n = 100000000;
    if (argc > 1)
    {
        n = atol(argv[1]);
    }

    const double step = 1.0 / (double)n;
    double sum = 0.0;
    double start = omp_get_wtime();

#pragma omp parallel for reduction(+ : sum)
    for (long i = 0; i < n; ++i)
    {
        double x = ((double)i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = step * sum;
    double finish = omp_get_wtime();

    printf("pi = %.16f\n", pi);
    printf("N = %ld\n", n);
    printf("threads = %d\n", omp_get_max_threads());
    printf("time = %.6f seconds\n", finish - start);
    return 0;
}
