#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Profiler.h"

Profiler prof("Functions");

int fact(int n)
{
    if(n)
    {
        return n * fact(n - 1);
    }

    return 1;
}

void createGroup(int step, int max_size)
{
    for(int n = 1; n < max_size; n += step)
    {
        prof.countOperation("100_log_n", n, 100 * log(n));
        prof.countOperation("2_n", n, pow(2, n));
        prof.countOperation("2_n_fact", n, 2 * fact(n));
    }

    for(int n = 1; n < max_size * 3; n += step)
    {
        prof.countOperation("n", n, n);
        prof.countOperation("10_n", n, 10 * n);
        prof.countOperation("0.5_n_2", n, 0.5 * n * n);
    }

    prof.createGroup("Functions_1", "n",  "10_n", "0.5_n_2");
    prof.createGroup("Functions_2", "2_n_fact");
    prof.createGroup("Functions_3", "100_log_n", "2_n");
    prof.showReport();
    prof.reset();
}

int main()
{
    createGroup(1, 10);
    createGroup(1, 5);
    createGroup(1, 12);

    return 0;
}
