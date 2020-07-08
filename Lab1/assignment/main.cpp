#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "Profiler.h"

#define MAX_SIZE 100000

/** INSERT COMMENTARY HERE FOR 10*/

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

/** Selection sort */
void selectSort(int a[], int n)
{
    for(int i = 0; i < n - 1; i++) //main loop
    {
        //count assignments, operations
        /*Operation A = p.createOperation("attr", i);
        Operation O = p.createOperation("op", i);*/

        //at each step select the index of minimum from the remaining items
        int min = INT_MAX;
        for(int j = i; j < n; j++)
        {
            //O.count(1);
            //search min
            if(a[j] < a[min])
            {
                min = j;
            }
        }

        //A.count(3);
        //swap next element with the minimum
        int temp = a[i];
        a[i] = a[min];
        a[min] = temp;
    }
}

void insertionSort(int a[], int n)
{
    assignments = 0;
    comparisons = 0;
    all = 0;

    for(int i = 1; i < n; i++)
    {
        ///Operation C = p.createOperation("Comparations", i);
        ///Operation A = p.createOperation("Assignments", i);

        int buf = a[i];
        int j = i - 1;

        assignments++;

        //print_A(a, n);

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

void bubbleSort(int a[], int n)
{
    //compare two consecutive elements, if an element is not on the correct position swap them
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(a[j] > a[j + 1])
            {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

void best_ins_sort()
{
    //p.reset();

    for(int n = 100; n <= 10000; n += 100)
    {
        FillRandomArray(a, n, 0, 1000, false, ASCENDING);

        insertionSort(a, n);

        p.countOperation("ins_assign_best", n, assignments);
        p.countOperation("ins_comp_best", n, comparisons);
    }

    p.addSeries("ins_all_best", "ins_assign_best", "ins_comp_best");
    p.createGroup("ins_sort_best", "ins_all_best", "ins_assign_best", "ins_comp_best");
}

void avg_ins_sort()
{
    //p.reset();

    for(int n = 100; n <= 10000; n += 100)
    {
        FillRandomArray(a, n, 0, 1000, false, UNSORTED);

        insertionSort(a, n);

        p.countOperation("ins_assign_avg", n, assignments);
        p.countOperation("ins_comp_avg", n, comparisons);
    }

    p.addSeries("ins_all_avg", "ins_assign_avg", "ins_comp_avg");
    p.createGroup("ins_sort_avg", "ins_all_avg", "ins_assign_avg", "ins_comp_avg");

}

void worst_ins_sort()
{
    //p.reset();

    for(int n = 100; n <= 10000; n += 100)
    {
        FillRandomArray(a, n, 0, 1000, false, DESCENDING);

        insertionSort(a, n);

        p.countOperation("ins_assign_worst", n, assignments);
        p.countOperation("ins_comp_worst", n, comparisons);
    }

    p.addSeries("ins_all_worst", "ins_assign_worst", "ins_comp_worst");
    p.createGroup("ins_sort_worst", "ins_all_worst", "ins_assign_worst", "ins_comp_worst");
}

int main()
{
    best_ins_sort();
    worst_ins_sort();
    avg_ins_sort();

    p.createGroup("insertion_sort_assignments", "ins_assign_best", "ins_assign_avg", "ins_assign_worst");

    p.showReport();

    return 0;
}
