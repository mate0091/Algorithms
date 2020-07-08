#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "Profiler.h"

#define MAX_SIZE 100000

/** INSERT COMMENTARY HERE FOR 10*/
/**
    Observations:
    1. It takes unusually long to generate all the charts

    Comparing the sorts:
    Bubble sort seems to be the least efficient O(n^2) sort, but it can be useful in big data, when only 2 items need to be loaded into RAM

    The main similarity lies between insertion and selection sort. In the case of selection sort, the part that is already sorted consists of the smallest values from the entire array,
    but in insertion sort, these are the first items from the input. Because of this, insertion sort is useful when the input data is almost sorted. However, insertion sort performs a
    larger amount of memory manipulations (most i+1 swaps for i iterations -> O(n^2)) than selection sort (3 swaps -> O(n), stable).

    Binary search is a huge improvement for insertion sort, because to find where the item fits, linear search takes O(n) time, but binary O(log(n))
*/

int a[MAX_SIZE] = {0};


Profiler p("Sorting-evaluation");

int assignments, comparisons, all;

void print_A(int a[], int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

/** Selection sort

    It is the sorting algorithm, which goes through the array, at each step selecting the minimum of the remaining items,
    and swapping them with the current one

    Behavior:
                Assignments     Comparisons
    Best case:      3               n-i+1 -> O(n^2)
    Worst case:     3               n-i+1

    Stability: This algorithm is unstable, because if there is at least one value identical with the value at the current
    iteration and the minimum comes after it, the minimum will get swapped with the first item, and the order of the
    identical values is broken.
 */
void selectSort(int a[], int n)
{
    assignments = 0;
    comparisons = 0;

    for(int i = 0; i < n - 1; i++) //main loop
    {
        //count assignments, operations

        //at each step select the index of minimum from the remaining items
        int min = i;
        assignments++;

        print_A(a, n);

        for(int j = i; j < n; j++)
        {
            //search min
            comparisons++;

            if(a[j] < a[min])
            {
                min = j;
            }
        }

        assignments += 3;
        //swap next element with the minimum
        int temp = a[i];
        a[i] = a[min];
        a[min] = temp;
    }
}

/** Insertion sort

    It is the sorting algorithm, which goes through the array, inserts the item at the current iteration into its correct place
    building the sorted array one item at a time

    Behavior:
                Assignments     Comparisons
    Best case:      2               1
    Worst case:     i+2             i+1 -> O(n^2)

    Stability: This algorithm is stable, because of the strict "<" relation between the rest of the array and the buffer.
    The current item will not go backwards if it reaches a value equal with it.
 */
void insertionSort(int a[], int n)
{
    assignments = 0;
    comparisons = 0;

    for(int i = 1; i < n; i++)
    {
        int buf = a[i];
        int j = i - 1;

        assignments++;

        print_A(a, n);

        for(; a[j] > buf && j >= 0; j--) //shift the whole array until the correct position of the buffer is reached
        {
            comparisons++;

            a[j + 1] = a[j];

            assignments++;
        }

        comparisons++;

        a[j + 1] = buf;

        assignments++;
    }
}

/** Bubble sort

    It is the sorting algorithm, which goes through the array, and swaps two adjacent items, is their order is not correct.
    The maximum value is guaranteed to "bubble up" to the end, when an iteration is finished. It does this n-1 times, so that
    each value has a chance to reach its correct position when the algorithm finishes -> O(n^2)

    Behavior:
                Assignments     Comparisons
    Best case:      0               i
    Worst case:     3i              i

    Stability: This algorithm is stable by nature (swap doesn't occur for equal items)
 */
void bubbleSort(int a[], int n)
{
    assignments = 0;
    comparisons = 0;

    //compare two consecutive elements, if an element is not on the correct position swap them
    for(int i = 0; i < n - 1; i++)
    {
        print_A(a, n);

        for(int j = 0; j < n - 1; j++)
        {
            comparisons++;

            if(a[j] > a[j + 1])
            {
                assignments += 3;

                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

/** Algorithm for the binary search, modified so it finds where the selected value would belong

*/
int binSearch(int a[], int x, int l, int r)
{
    comparisons++;
    if(l >= r) //value is not found, check if it belongs to left or right of the selected item
    {
        comparisons++;
        if (x > a[l]) return l + 1;
        else return l;
    }

    int m = (l + r) / 2; //find middle index

    if(a[m] == x) return m + 1;  //in case of identical items, return middle + 1 to ensure stability

    else //search in left or right subarray
    {
        comparisons++;
        if(x > a[m]) return binSearch(a, x, m + 1, r);
        else return binSearch(a, x, l, m);
    }
}

/** Binary insertion sort
    Same as linear insertion sort, but uses binary search to find where the item at the current iteration belongs

    Behavior:
                Assignments     Comparisons
    Best case:      2               log2(i)
    Worst case:     i+2             log2(3i) -> still O(n^2) efficiency

    Stability: This algorithm is still stable, because in the binary search if the value is found, the selected position will be found + 1.
*/
void BinsertionSort(int a[], int n)
{
    assignments = 0;
    comparisons = 0;

    for(int i = 1; i < n; i++)
    {
        int buf = a[i];
        int j = i - 1;

        assignments++;

        print_A(a, n);

        //search the correct place for the buffer

        int k = binSearch(a, buf, 0, j); //search in the already sorted part

        for(; j >= k; j--) //shift the whole array until j < k
        {
            a[j + 1] = a[j];

            assignments++;
        }

        comparisons++;

        a[j + 1] = buf;

        assignments++;
    }
}

/** This is the function that generates one chart with a given case and a given type of sort
    Accepts the name of the functions and the name of the group as parameters
*/
void sort_eval(void (*sort_func)(int*, int), SortMethod method, char asg[50], char cmp[50], char all[50], char group_name[50])
{
    //p.reset();

    for(int n = 100; n <= 10000; n += 100)
    {
        if(method == UNSORTED) //if calculating average
        {
            //calculate average from 5 tries
            for(int i = 0; i < 5; i++)
            {
                FillRandomArray(a, n, 0, 1000, false, method);

                sort_func(a, n);

                //assignments /= 5;
                //comparisons /= 5;

                p.countOperation(asg, n, assignments);
                p.countOperation(cmp, n, comparisons);
            }
        }

        else
        {
            FillRandomArray(a, n, 0, 1000, false, method);

            sort_func(a, n);

            p.countOperation(asg, n, assignments);
            p.countOperation(cmp, n, comparisons);
        }
    }

    if(method == UNSORTED)
    {
        p.divideValues(asg, 5);
        p.divideValues(cmp, 5);
    }

    p.addSeries(all, asg, cmp);
    p.createGroup(group_name, all, asg, cmp);
}

int main()
{
   /* sort_eval(insertionSort, UNSORTED, "ins_assign_avg", "ins_comp_avg", "ins_all_avg", "ins_sort_avg");
    sort_eval(insertionSort, ASCENDING, "ins_assign_best", "ins_comp_best", "ins_all_best", "ins_sort_best");
    sort_eval(insertionSort, DESCENDING, "ins_assign_worst", "ins_comp_worst", "ins_all_worst", "ins_sort_worst");

    p.createGroup("insertion_sort", "ins_all_best", "ins_all_avg", "ins_all_worst");

    sort_eval(selectSort, UNSORTED, "sel_assign_avg", "sel_comp_avg", "sel_all_avg", "sel_sort_avg");
    sort_eval(selectSort, ASCENDING, "sel_assign_best", "sel_comp_best", "sel_all_best", "sel_sort_best");
    sort_eval(selectSort, DESCENDING, "sel_assign_worst", "sel_comp_worst", "sel_all_worst", "sel_sort_worst");

    p.createGroup("selection_sort", "sel_all_best", "sel_all_avg", "sel_all_worst");

    sort_eval(bubbleSort, UNSORTED, "bub_assign_avg", "bub_comp_avg", "bub_all_avg", "bub_sort_avg");
    sort_eval(bubbleSort, ASCENDING, "bub_assign_best", "bub_comp_best", "bub_all_best", "bub_sort_best");
    sort_eval(bubbleSort, DESCENDING, "bub_assign_worst", "bub_comp_worst", "bub_all_worst", "bub_sort_worst");

    p.createGroup("bubble_sort", "bub_all_best", "bub_all_avg", "bub_all_worst");

    p.createGroup("Best case assignments", "ins_assign_best", "sel_assign_best", "bub_assign_best");
    p.createGroup("Best case comparisons", "ins_comp_best", "sel_comp_best", "bub_comp_best");
    p.createGroup("Best case all operations", "ins_all_best", "sel_all_best", "bub_all_best");

    p.createGroup("Worst case assignments", "ins_assign_worst", "sel_assign_worst", "bub_assign_worst");
    p.createGroup("Worst case comparisons", "ins_comp_worst", "sel_comp_worst", "bub_comp_worst");
    p.createGroup("Worst case all operations", "ins_all_worst", "sel_all_worst", "bub_all_worst");

    p.createGroup("Avg case assignments", "ins_assign_avg", "sel_assign_avg", "bub_assign_avg");
    p.createGroup("Avg case comparisons", "ins_comp_avg", "sel_comp_avg", "bub_comp_avg");
    p.createGroup("Avg case all operations", "ins_all_avg", "sel_all_avg", "bub_all_avg");

    sort_eval(insertionSort, UNSORTED, "ins_assign_avg", "ins_comp_avg", "ins_all_avg", "ins_sort_avg");
    sort_eval(insertionSort, ASCENDING, "ins_assign_best", "ins_comp_best", "ins_all_best", "ins_sort_best");
    sort_eval(insertionSort, DESCENDING, "ins_assign_worst", "ins_comp_worst", "ins_all_worst", "ins_sort_worst");

    sort_eval(BinsertionSort, UNSORTED, "Bins_assign_avg", "Bins_comp_avg", "Bins_all_avg", "Bins_sort_avg");
    sort_eval(BinsertionSort, ASCENDING, "Bins_assign_best", "Bins_comp_best", "Bins_all_best", "Bins_sort_best");
    sort_eval(BinsertionSort, DESCENDING, "Bins_assign_worst", "Bins_comp_worst", "Bins_all_worst", "Bins_sort_worst");

    p.createGroup("insertion_sorts_best", "ins_all_best", "Bins_all_best");
    p.createGroup("insertion_sorts_avg", "ins_all_avg", "Bins_all_avg");
    p.createGroup("insertion_sorts_worst", "ins_all_worst", "Bins_all_worst");

    p.showReport();*/

    FillRandomArray(a, 10, 0, 99, false, UNSORTED);

    selectSort(a, 10);
    //insertionSort(a, 10);
    //bubbleSort(a, 10);
    //BinsertionSort(a, 10);

    print_A(a, 10);

    return 0;
}
