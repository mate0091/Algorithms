#include "Profiler.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100000
// bonus: quick sort iteratively and recursively

//

/**

INSERT COMMENTARY HERE

Quick sort is a sorting algorithm based on the divide et impera paradigm. It has a very important subroutine: the partitioning method.
Here is the 3-step process for sorting an array using quick sort:

1. Divide:  Select a pivot, then rearrange (partition) the array into 2 subarrays such that each element from the left subarray is less than or equal to the pivot,
            and each element from the right subarray is greater than or equal to the pivot
2. Conquer: Sort the two subarrays recursively
3. Combine: Do nothing, as if the two subarrays are already sorted and the elements of the left subarray are less than the pivot and the element of the right subarray
            are greater than the pivot, and the pivot divides them, then the whole array is sorted

The partitioning method runs in O(n) time, as each element is compared once with the pivot. There are many different ways of choosing the pivot, but the most common is selecting the last item.
This does not present any issues as long as the initial array is completely random. The running time of the quicksort algorithm depends on how equally the problem is distributed after a call to
the partition method. The best case scenario is when the pivot is exactly the median of the array, so that two equal subproblems need be resolved (or with a difference of 1, depending
on the parity of the size). This way, the call tree will look like a perfectly balanced binary tree with a height of logn, that means O(n*logn) running time for best case. To generate a best
case for quick sort, I used a median pivot and a sorted array, so that the problem is as balanced as it could be. Similarly, the worst case occurs when the problem of size n is distributed into
a subproblem with size n - 1 and another one of 0, so when selecting the first or last element as pivot with the array already sorted, quick sort runs in O(n^2) time, just as slow as any direct
sorting method (no item/all items are smaller/greater than the pivot).

The quick select algorithm returns the ith smallest element from an array, and it works very similarly to quick sort. It is based on the fact that the pivot is guaranteed to reach its correct place
after a call to partition. The recursive call depends on which side of the randomly selected pivot our searched item is. If the searched index is less then the pivot we call on the left subarray,
if it is greater, on the right subarray.

The average case lies very close to the best case for quicksort as it can be seen on the charts. Its function's shape resembles an n*logn curve, with very small noise, since for organically random
data, it is should be sheer luck how equally the selected pivot divides the problem. The worst case can be easily avoided by selecting a random partition. Thus quick sort can be very fast for
all kinds of data.

Comparing with heap sort:
For the worst case, heap sort is the better with a guaranteed O(n*logn) complexity. However, quick sort still wins in the best case, because there are no swaps for already ordered data. In turn,
heap sort has to do all its subroutines: build the heap, extract max, heapify. They all use memory manipulation, which is costly. Quick sort also swaps parts of data on the average case,
but not all of it, like heap sort. The worst case of quick sort can be easily eliminated, therefore quick sort is faster on random data, than heap sort.

*/
Profiler p("QuickSort - HeapSort");

int arr[MAX_SIZE];
int ops;

int heapsize = 0;

void print_A(int a[], int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int part_rand(int a[], int l, int r);
int part(int a[], int l, int r);
void quicksort(int a[], int (*part_func)(int*, int, int), int l, int r, bool flag);
int quickselect(int a[], int l, int r, int i, bool flag);

int part_rand(int a[], int l, int r)
{
    srand(time(0));

    int pivot = rand() % (r - l) + l;

    //printf("Pivot index: %d, pivot: %d\n\n", pivot, arr[pivot]);

    std::swap(a[pivot], a[r]); //swap pivot with last element, now a[r] is the pivot

    ops += 3;

    int i = l - 1;

    for(int j = l; j < r; j++)
    {
        ops++;
        if(a[j] < a[r]) //then swap
        {
            ops += 3;
            i++;
            std::swap(a[i], a[j]);
        }
    }

    ops += 3;
    std::swap(a[i + 1], a[r]);

    return i + 1;
}

int part(int a[], int l, int r)
{
    int pivot = a[r];
    ops += 1;

    int i = l - 1;

    for(int j = l; j < r; j++)
    {
        ops++;
        if(a[j] < pivot) //then swap
        {
            ops += 3;
            i++;
            std::swap(a[i], a[j]);
        }
    }

    ops += 3;
    std::swap(a[i + 1], a[r]);

    return i + 1;
}

int part_m(int a[], int l, int r)
{
    int pivot = (l + r) / 2;

    std::swap(a[pivot], a[r]); //swap pivot with last element, now a[r] is the pivot

    ops += 3;

    int i = l - 1;

    for(int j = l; j < r; j++)
    {
        ops++;
        if(a[j] < a[r]) //then swap
        {
            ops += 3;
            i++;
            std::swap(a[i], a[j]);
        }
    }

    ops += 3;
    std::swap(a[i + 1], a[r]);

    return i + 1;
}

void quicksort(int a[], int (*part_func)(int*, int, int),int l, int r, bool flag)
{
    if(l < r)
    {
        if(flag) print_A(a, 10);

        int pivotIndex = part_func(a, l, r);

        quicksort(a, part_func, l, pivotIndex - 1, flag);
        quicksort(a, part_func, pivotIndex + 1, r, flag);
    }
}

int quickselect(int a[], int l, int r, int i, bool flag)
{
    if(l == r) return a[l];

    int q = part_rand(a, l, r);
    int k = q - l + 1;

    if(flag)
    {
        print_A(a, 10);
        printf("pivot: %d\n", a[q]);
    }

    if(i == k) return a[q];

    else if(i < k) return quickselect(a, l, q - 1, i, flag);
    return quickselect(a, q + 1, r, i - k, flag);
}

int parent(int i)
{
    return (i - 1) /2;
}

int left(int i)
{
    return 2 * i + 1;
}

int right(int i)
{
    return 2 * i + 2;
}

void heapify_down(int h[], int i) //sink the root into the heaviest branch
{
    int largest_ind;
    //swap largest child with node
    if(left(i) < heapsize && h[left(i)] > h[i])
    {
        largest_ind = left(i);
    }

    else largest_ind = i;

    if (right(i) < heapsize && h[right(i)] > h[largest_ind]) largest_ind = right(i);

    ops += 4;
    if(largest_ind != i)
    {
        std::swap(h[i], h[largest_ind]);
        ops += 3;

        heapify_down(h, largest_ind);
    }
}

void build_max_heap_bottom_up(int h[], int n)
{
    heapsize = n;

    for(int i = (n/2); i >= 0; i--)
    {
        heapify_down(h, i);
    }
}

void heapsort(int h[], int n)
{
    build_max_heap_bottom_up(h, n);

    for(int i = n - 1; i >= 1; i--)
    {
        std::swap(h[0], h[i]);

        heapsize--;

        heapify_down(h, 0);
    }
}

void qSort_test()
{
    printf("QUICKSORT TEST:\n");
    FillRandomArray(arr, 10, 0, 20, false, UNSORTED);
    quicksort(arr, part, 0, 9, true);

    print_A(arr, 10);

    printf("\n\n");
}

void qSelect_test(int index)
{
    printf("QUICKSELECT TEST:\n");

    FillRandomArray(arr, 10, 0, 20, false, UNSORTED);
    print_A(arr, 10);

    int k = quickselect(arr, 0, 9, index, true);

    printf("The %dth element of the sorted array is: %d\n\n", index, k);
}

void avg_case(char operationsQuickSort[50], char operationsHeapSort[50])
{
    int heap[MAX_SIZE] = {0};

    for(int n = 100; n <= 10000; n += 100)
    {
        for(int i = 0; i < 5; i++)
        {
            FillRandomArray(arr, n, 0, 1000, false, UNSORTED);
            CopyArray(heap, arr, n);

            ops = 0;
            quicksort(arr, part, 0, n - 1, false);
            p.countOperation(operationsQuickSort, n, ops);

            ops = 0;
            heapsort(heap, n);
            p.countOperation(operationsHeapSort, n, ops);
        }
    }

    p.divideValues(operationsQuickSort, 5);
    p.divideValues(operationsHeapSort, 5);
}

void eval_qsort(int c, char operationsQuickSort[50])
{
    for(int n = 100; n <= 10000; n += 100)
    {
        FillRandomArray(arr, n, 0, 1000, false, DESCENDING);

        ops = 0;
        quicksort(arr, (c == 1)? part_m: part, 0, n - 1, false);
        p.countOperation(operationsQuickSort, n, ops);
    }
}

int main()
{
    avg_case("quicksort_avg", "heapsort_avg");
    p.createGroup("Comparison", "quicksort_avg", "heapsort_avg");
    qSort_test();
    qSelect_test(5);

    eval_qsort(1, "quicksort_best");
    eval_qsort(0, "quicksort_worst");

    p.createGroup("Quicksort", "quicksort_best", "quicksort_worst", "quicksort_avg");
    p.createGroup("Quicksort_best_case_vs_avg_case", "quicksort_best", "quicksort_avg");

    p.showReport();

    return 0;
}
