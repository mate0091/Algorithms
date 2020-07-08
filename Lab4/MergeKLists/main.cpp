#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#include "SLList.h"

#define MAX_SIZE 100000

/**COMMENTARY HERE

This is an implementation of the K List Merge algorithm using heaps.

A naive solution for this problem would be to take the first list and starting from the second, insert the elements into the list. Then repeat this with the rest of the arrays.
This is not an efficient solution, since it takes O(n^2*k^2) time. An efficient solution is implemented here that can merge K lists in O(n*k*log(k)) time using a min heap.
The algorithm works as follows:
1.  Create the output list and insert the first elements from all the lists into a min heap
2.  Pop minimum element and insert it into the list, and replace the element with the first element from the list from which we popped. Repeat n*k times.
The maximum heap size is k, pushing and popping into a heap is done in O(log(k)) time.

On the charts we can see that the amount of lists to merge influences heavily the running time of the algorithm, which is expected.
The curve on the second chart represents a logarithmic one, with small deviations.

*/

Profiler p("KListMerge");

int ops;

int arr[MAX_SIZE] = {0};
SLList Lists[10000];
SLList output;

int heapsize = 0;

typedef struct
{
    int content;
    int source;
} Heap_elem;

 Heap_elem h[MAX_SIZE];

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

void print_A(Heap_elem h[], int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("%d ", h[i].content);
    }
    printf("\n");
}

void array_to_SLL(int a[], int n, SLList* List)
{
    for(int i = 0; i < n; i++)
        insertAtRear(List, a[i]);
}

void heapify_down(Heap_elem h[], int i) //sink the root into the heaviest branch
{
    int min_ind;
    //swap largest child with node
    if(left(i) < heapsize && h[left(i)].content < h[i].content)
    {
        min_ind = left(i);
    }

    else min_ind = i;

    if (right(i) < heapsize && h[right(i)].content < h[min_ind].content) min_ind = right(i);

    ops += 2;

    if(min_ind != i)
    {
        ops += 3;
        std::swap(h[i], h[min_ind]);

        heapify_down(h, min_ind);
    }
}

void heapify_up(Heap_elem h[], int i) //bubble up the index
{
    //if parent exists, and is bigger then index, swap!
    ops += 1;
    if(parent(i) >= 0 && h[parent(i)].content > h[i].content)
    {
        ops += 3;
        std::swap(h[i], h[parent(i)]);
        //walk upwards
        heapify_up(h, parent(i));
    }
}

void push_heap(Heap_elem h[], int x, int s)
{
    //add element to the last spot
    h[heapsize].content = x;
    h[heapsize].source = s;
    ops += 2;

    //increase heap size
    heapsize++;

    //heapify
    heapify_up(h, heapsize - 1);
}

Heap_elem pop_heap(Heap_elem h[])
{
    Heap_elem val = h[0];
    std::swap(h[0], h[heapsize - 1]);
    ops += 3;

    heapsize--;

    heapify_down(h, 0);

    return val;
}

void mergeList(SLList* list1, SLList* list2, SLList* dest)
{
    initList(dest);

    Node* w1 = list1->first;
    Node* w2 = list2->first;

    while(w1 != NULL && w2 != NULL)
    {
        if(w1->content < w2->content)
        {
            //add w1->id to new list, increment walker
            insertAtRear(dest, w1->content);
            w1 = w1->next;
        }
        else
        {
            insertAtRear(dest, w2->content);
            w2 = w2->next;
        }
    }

    //add rest of the lists
    while(w1 != NULL)
    {
        insertAtRear(dest, w1->content);
        w1 = w1->next;
    }

    while(w2 != NULL)
    {
        insertAtRear(dest, w2->content);
        w2 = w2->next;
    }
}

void generate_Lists(int n, int k) //array of SLList pointers
{
    for(int i = 0; i < n % k ; i++)
    {
        initList(&Lists[i]);
        FillRandomArray(arr, n/k + 1, 0, 10000, false, ASCENDING);
        array_to_SLL(arr, n/k + 1, &Lists[i]);
    }

    for(int i = n % k; i < k ; i++)
    {
        initList(&Lists[i]);
        FillRandomArray(arr, n/k, 0, 10000, false, ASCENDING);
        array_to_SLL(arr, n/k, &Lists[i]);
    }
}

void test_merge()
{
    SLList list1, list2;
    initList(&list1);
    initList(&list2);

    FillRandomArray(arr, 20, 0, 99, true, ASCENDING);
    array_to_SLL(arr, 20, &list1);

    FillRandomArray(arr, 20, 0, 99, true, ASCENDING);
    array_to_SLL(arr, 20, &list2);

    printf("List1: ");
    printList(&list1);
    printf("List2: ");
    printList(&list2);

    SLList list3;
    mergeList(&list1, &list2, &list3);

    printf("List3: ");
    printList(&list3);
}

int random(int x, int y)
{
    return (rand() % (y - x)) + x;
}

void merge_k_lists(int k, int n, int flag) //k nr of lists, n length
{
    generate_Lists(n, k);

    if(flag)
    {
        for(int i = 0; i < k; i++)
        {
            printf("List%d: ", i);
            printList(&Lists[i]);
        }
    }

    for(int i = 0; i < k; i++)
    {
        push_heap(h, Lists[i].first->content, i);
    }

    for(int i = 0; i < n; i++)
    {
        ops++;
        Heap_elem elem = pop_heap(h);

        if(flag) insertAtRear(&output, elem.content);

        deleteAtFront(&Lists[elem.source]);

        //if list from source is empty, go from beginning to find a list where it is not empty
        if(Lists[elem.source].first)
        {
            push_heap(h, Lists[elem.source].first->content, elem.source);
        }
    }
}

void merge_eval(int k, char function_name[50])
{
    for(int i = 0; i < 5; i++)
    {
        for(int n = 100; n <= 10000; n += 100)
        {
            ops = 0;
            merge_k_lists(k, n, 0);

            p.countOperation(function_name, n, ops);
        }
    }

    p.divideValues(function_name, 5);
}

void merge_eval(char function_name[])
{
    for(int i = 0; i < 5; i++)
    {
        for(int k = 10; k <= 500; k += 10)
        {
            ops = 0;
            merge_k_lists(k, 10000, 0);

            p.countOperation(function_name, k, ops);
        }
    }

    p.divideValues(function_name, 5);
}

void evaluate()
{
    merge_eval(5, "five");
    merge_eval(10, "ten");
    merge_eval(100, "one_hundred");

    p.createGroup("Merging_in_function_of_n", "five", "ten", "one_hundred");

    merge_eval("n_ten_thousand");
    p.createGroup("Merging_in_function_of_k", "n_ten_thousand");

    p.showReport();
}

void test_k_merge()
{
    initList(&output);
    merge_k_lists(4, 10, 1);

    printf("Output: ");
    printList(&output);
    printf("\n");
}

int main()
{
    evaluate();
    return 0;
}
