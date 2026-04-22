#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    long n = 1000000;
    if (argc > 1)
    {
        n = atol(argv[1]);
    }

    const double true_a = 2.0;
    const double true_b = 5.0;

    double sum_x = 0.0;
    double sum_y = 0.0;
    double sum_xx = 0.0;
    double sum_xy = 0.0;
    double start = omp_get_wtime();

#pragma omp parallel for reduction(+ : sum_x, sum_y, sum_xx, sum_xy)
    for (long i = 0; i < n; ++i)
    {
        double x = (double)i / (double)n;
        double noise = 0.01 * sin((double)i);
        double y = true_a * x + true_b + noise;

        sum_x += x;
        sum_y += y;
        sum_xx += x * x;
        sum_xy += x * y;
    }

    double den = (double)n * sum_xx - sum_x * sum_x;
    double a = ((double)n * sum_xy - sum_x * sum_y) / den;
    double b = (sum_y - a * sum_x) / (double)n;

    double err = 0.0;
#pragma omp parallel for reduction(+ : err)
    for (long i = 0; i < n; ++i)
    {
        double x = (double)i / (double)n;
        double y = true_a * x + true_b + 0.01 * sin((double)i);
        double r = y - (a * x + b);
        err += r * r;
    }

    double finish = omp_get_wtime();
    printf("n = %ld\n", n);
    printf("model: y = a*x + b\n");
    printf("a = %.10f\n", a);
    printf("b = %.10f\n", b);
    printf("sum squared residuals = %.10f\n", err);
    printf("threads = %d\n", omp_get_max_threads());
    printf("time = %.6f seconds\n", finish - start);
    return 0;
}
