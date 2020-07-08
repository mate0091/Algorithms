#include <stdio.h>
#include "Profiler.h"

#define MAX_SIZE 100000
/**
    Counting assignments and comparisons not required, just operations overall
*/

/** COMMENTARY GOES HERE */

/**
    The following code is a C implementation of the heap - manipulation algorithms as well as heap sort.

    A heap is a data structure, which derives from a binary tree. It has one additional property: In a subtree neither of the children are bigger than the root (In case of max - heap).
    This is a partial order relation, and a total order relation on an individual branch.
    There is no relationship between the siblings. To implement the heap data structure, I used a static array, where index i's children are at index 2*i (left) and 2 * i + 1 (right).
    There are two strategies to make a heap out of an arbitrary array:
    Bottom - up procedure:  Start from 2 single node heaps, and add a third node as a parent to build a 3 - heap (parent will be the maximum of the 3 items), applying heapify to the
                            first non - leaf node (size / 2) and continue building up the heap for the left sibling of the processed node (i - 1 th index) until we reach the root.
                            It uses the heapify down method ("sink")
    Top - down procedure:   Start from a 1 - heap, and continue inserting nodes into the heap from the array, until the last element from the array is inserted. (using "bubble up" heapify)

    Heapify down:           select the biggest item from a node and its two children, and if their order is not correct, swap the node with the largest item and continue from the largest
                            item downwards. Stop when no swap is needed.
    Heapify up:             select a biggest item from a node and its parent, if a swap is needed, perform it and continue upwards.

    Push into heap:         Puts the item into the last place of the heap, then bubbles it up to its correct position.

    The complexity of the bottom - up approach is O(n), and O(n*logn) of the top - down. This can be seen very well on the charts, where the top - down approach lags behind.
    Because of this, the bottom - up strategy is much more desirable for heap sort. It has one major drawback, however. It cannot be used for heaps with varying size, since we need the
    starting point from where the nodes are not leaves, therefore we need the maximum heap size. For this purpose the top - down approach is used which deals with the push - pop method.
    Pushing and popping are basic operations for a heap, both have O(logn) complexity, but heap sort performs worse this way. Still, this approach is very good when we have operations
    already implemented on a data structure, such as a priority queue. But if the data structure is allocated dynamically, more operations on the memory are needed, so the top - down strategy
    can be slow when implemented on EEPROMs for example. In the end, the choice of a method over the other depends on the situation: Is it used only for sorting? How well can the hardware handle
    memory operations? How much does efficiency matter?, ...

*/

Profiler p("Heap sort");
int tdo, buo;

int heap[MAX_SIZE] = {0};

int heapsize = 0;

int parent(int i);
int left(int i);
int right(int i);

void print_A(int a[], int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void heapify_up(int h[], int i);
void heapify_down(int h[], int i);

void build_max_heap_bottom_up(int h[], int i, bool print);
void build_max_heap_top_down(int h[], int i, bool print);

void heapsort(int h[], int n);

void push_heap(int h[], int x);

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

    buo += 2;
    if(largest_ind != i)
    {
        std::swap(h[i], h[largest_ind]);
        buo += 3;

        heapify_down(h, largest_ind);
    }
}

void heapify_up(int h[], int i) //bubble up the index
{
    //if parent exists, and is bigger then index, swap!
    tdo += 2;
    if(parent(i) >= 0 && h[parent(i)] < h[i])
    {
        std::swap(h[i], h[parent(i)]);
        tdo += 3;

        //walk upwards
        heapify_up(h, parent(i));
    }
}

void build_max_heap_bottom_up(int h[], int n, bool print)
{
    heapsize = n;

    for(int i = (n/2); i >= 0; i--)
    {
        if(print) print_A(h, n);

        heapify_down(h, i);
    }
}

void build_max_heap_top_down(int h[], int n, bool print = false)
{
    heapsize = 1;

    for(int i = 1; i < n; i++)
    {
        if(print) print_A(h, n);

        push_heap(h, h[i]);
    }
}

void heapsort(int h[], int n)
{
    build_max_heap_bottom_up(h, n, false);

    for(int i = n - 1; i >= 1; i--)
    {
        print_A(h, n);

        std::swap(h[0], h[i]);

        heapsize--;

        heapify_down(h, 0);
    }
}

void push_heap(int h[], int x)
{
    //add element to the last spot
    h[heapsize] = x;
    tdo++;

    //increase heap size
    heapsize++;

    //heapify
    heapify_up(h, heapsize - 1);
}

/** TEST FUNCTIONS GO HERE*/
void test()
{
    printf("BOTTOM UP BUILD TEST:\n");
    //test BU heap build
    FillRandomArray(heap, 10, 0, 9, true, UNSORTED);
    build_max_heap_bottom_up(heap, 10, true);
    print_A(heap, 10);

    printf("TOP DOWN BUILD TEST:\n");
    //test TD heap build
    FillRandomArray(heap, 10, 0, 9, true, ASCENDING);
    build_max_heap_top_down(heap, 10, true);
    print_A(heap, 10);

    printf("HEAPSORT TEST:\n");
    //test heap sort
    FillRandomArray(heap, 20, 0, 99, false, UNSORTED);
    print_A(heap, 20);
    heapsort(heap, 20);
    print_A(heap, 20);
}

/** EVALUATION FUNCTION GOES HERE*/
void eval(SortMethod method, char TD_name[50], char BU_name[50], char chart_name[50])
{
    int heap_beta[MAX_SIZE] = {0};

    for(int n = 100; n <= 10000; n += 100)
    {
        //calculate avg, if method is not worst case

        if(method == UNSORTED)
        {
            for(int k = 0; k < 5; k++)
            {
                //count operations and use the same input for the 2 methods
                FillRandomArray(heap, n, 0, 9999, false, method);

                CopyArray(heap_beta, heap, n);

                buo = 0;
                tdo = 0;

                build_max_heap_bottom_up(heap, n, false);
                //count operations
                p.countOperation(BU_name, n, buo);

                build_max_heap_top_down(heap_beta, n, false);

                p.countOperation(TD_name, n, tdo);
            }
        }

        else
        {
                //count operations and use the same input for the 2 methods
                FillRandomArray(heap, n, 0, 9999, false, method);

                CopyArray(heap_beta, heap, n);

                buo = 0;
                tdo = 0;

                build_max_heap_bottom_up(heap, n, false);
                //count operations
                p.countOperation(BU_name, n, buo);

                build_max_heap_top_down(heap_beta, n, false);

                p.countOperation(TD_name, n, tdo);
        }
    }

    if(method == UNSORTED)
    {
        p.divideValues(TD_name, 5);
        p.divideValues(BU_name, 5);
    }

    p.createGroup(chart_name, TD_name, BU_name);
}

int main()
{
    //eval(UNSORTED, "top_down_avg", "bottom_up_avg", "Average");
    //eval(ASCENDING, "top_down_worst", "bottom_up_worst", "Worst"); //worst case is ascending, because of max - heap
    test();
    //p.showReport();

    return 0;
}
