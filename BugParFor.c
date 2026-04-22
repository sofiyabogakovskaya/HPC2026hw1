#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    const size_t n = 100;
    const int chunk = 3;
    float a[n], b[n], c[n];

    for (int i = 0; i < (int)n; ++i)
    {
        a[i] = (float)i;
        b[i] = (float)i;
        c[i] = 0.0f;
    }

#pragma omp parallel for schedule(static, chunk) shared(a, b, c)
    for (int i = 0; i < (int)n; ++i)
    {
        int tid = omp_get_thread_num();
        c[i] = a[i] + b[i];
        printf("tid = %d, c[%d] = %f\n", tid, i, c[i]);
    }

    return 0;
}
