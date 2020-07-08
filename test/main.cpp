#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define MAX_SIZE 25

Profiler prof("test values: ");

void read_A(int a[], int n, FILE* in)
{

    for(int i = 0; i < n; i++)
    {
        fscanf(in, "%d", &a[i]);
    }
}

void print_A(int a[], int n, FILE* out)
{
    for(int i = 0; i < n; i++)
    {
        fprintf(out, "%d ", a[i]);
    }
}

void RNG(int* a, int n, int low, int high)
{
    srand(time(0));

    for(int i = 0; i < n; i++)
    {
        a[i] = (rand() % (high - low)) + low;
    }
}

void nCount()
{
    Operation o = prof.createOperation("nCount", 10);
    for(int n = 0; n <= 10; n++)
    {
        o.count();
    }
}

int main()
{
    int a[MAX_SIZE] = {0};
    int n;

    FILE* in = fopen("input.txt", "r");
    FILE* out = fopen("output.txt", "w");

    fscanf(in, "%d", &n);

    //read_A(a, n, in);
    //RNG(a, n, 0, 15);
    //FillRandomArray(a, n, 0, 100, false, UNSORTED);
    //FillRandomArray(a, n, 0, 100, false, ASCENDING);

    //print_A(a, n, out);

    for(int n = 100; n <= 10000; n+=100)
    {
        nCount();
    }

    prof.showReport();

    return 0;
}

