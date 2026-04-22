#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

float dotprod(float *a, float *b, size_t n)
{
    float sum = 0.0f;

#pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < (int)n; ++i)
    {
        int tid = omp_get_thread_num();
        sum += a[i] * b[i];
        printf("tid = %d i = %d\n", tid, i);
    }

    return sum;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    const size_t n = 100;
    float a[n], b[n];

    for (int i = 0; i < (int)n; ++i)
    {
        a[i] = (float)i;
        b[i] = (float)i;
    }

    float sum = dotprod(a, b, n);
    printf("Sum = %f\n", sum);
    return 0;
}
