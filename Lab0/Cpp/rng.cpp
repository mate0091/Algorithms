#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 50

//with random seed
void RNG(int a[], int n, int low, int high)
{
    srand(time(0));

    for(int i = 0; i < n; i++)
    {
        a[i] = (rand() % (high - low)) + low;
    }
}

void print_A(int a[], int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int main()
{
    int a[MAX_SIZE] = {0};
    int n;

    scanf("%d", &n);

    RNG(a, n, 10, 100);

    print_A(a, n);

    return 0;
}
