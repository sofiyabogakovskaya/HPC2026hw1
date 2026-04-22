#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

static double matrix_value(int i, int j, int n)
{
    if (i == j)
    {
        return 2.0 * n;
    }
    return 1.0;
}

static void make_right_part(double *b, int n)
{
#pragma omp parallel for
    for (int i = 0; i < n; ++i)
    {
        double row_sum = 0.0;
        for (int j = 0; j < n; ++j)
        {
            row_sum += matrix_value(i, j, n);
        }
        b[i] = row_sum;
    }
}

int main(int argc, char *argv[])
{
    int n = 1000;
    int max_iter = 2000;
    double eps = 1e-8;

    if (argc > 1)
    {
        n = atoi(argv[1]);
    }
    if (argc > 2)
    {
        max_iter = atoi(argv[2]);
    }

    double *x_old = (double *)calloc(n, sizeof(double));
    double *x_new = (double *)calloc(n, sizeof(double));
    double *b = (double *)malloc((size_t)n * sizeof(double));
    if (!x_old || !x_new || !b)
    {
        printf("memory error\n");
        return 1;
    }

    make_right_part(b, n);

    double start = omp_get_wtime();
    int iter;
    double diff = 0.0;

    for (iter = 0; iter < max_iter; ++iter)
    {
        diff = 0.0;

#pragma omp parallel for reduction(max : diff)
        for (int i = 0; i < n; ++i)
        {
            double sigma = 0.0;
            for (int j = 0; j < n; ++j)
            {
                if (j != i)
                {
                    sigma += matrix_value(i, j, n) * x_old[j];
                }
            }
            x_new[i] = (b[i] - sigma) / matrix_value(i, i, n);
            double local_diff = fabs(x_new[i] - x_old[i]);
            if (local_diff > diff)
            {
                diff = local_diff;
            }
        }

#pragma omp parallel for
        for (int i = 0; i < n; ++i)
        {
            x_old[i] = x_new[i];
        }

        if (diff < eps)
        {
            ++iter;
            break;
        }
    }

    double max_error = 0.0;
#pragma omp parallel for reduction(max : max_error)
    for (int i = 0; i < n; ++i)
    {
        double err = fabs(x_old[i] - 1.0);
        if (err > max_error)
        {
            max_error = err;
        }
    }

    double finish = omp_get_wtime();
    printf("n = %d\n", n);
    printf("iterations = %d\n", iter);
    printf("last difference = %.12e\n", diff);
    printf("max error = %.12e\n", max_error);
    printf("threads = %d\n", omp_get_max_threads());
    printf("time = %.6f seconds\n", finish - start);

    free(x_old);
    free(x_new);
    free(b);
    return 0;
}
