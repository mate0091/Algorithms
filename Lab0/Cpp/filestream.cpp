#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 50

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
    fprintf(out, "\n");
}

int main()
{
    FILE* in = fopen("input.txt", "r");
    FILE* out = fopen("output.txt", "w");

    int a[MAX_SIZE] = {0};
    int n;
    fscanf(in, "%d", &n);

    read_A(a, n, in);

    print_A(a, n, out);

    return 0;
}
