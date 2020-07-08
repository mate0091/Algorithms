#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#include <string.h>

#define TABLE_SIZE 10007

/**
COMMENTARY HERE

This is an implementation of a hash table with open addressing and quadratic probing.

A hash table is a data structure that is incredibly efficient at handling the search operation. It is often used for large data applications, as instruction lookup tables for programming
languages etc.
Each element of the data structure has a key as a unique identifier. A hash function is a function that maps these keys to places in the table. When two distinct keys map to the same value,
a collision occurs and there are several ways to handle it. The first way is to treat the hash table like an array of buckets, each bucket being a list of the values that were mapped into it.
However, at worst, searching could take no less than searching in a linked list when implementing a hash table this way. Another way to handle collisions is resolution by open addressing.
With open addressing, all elements occupy the hash table itself. When searching for an element, we examine positions in the table until we find the desired element or we are sure that the
element is not in the table. In open addressing the table can fill up since no lists are used for additional space. It computes a probe sequence, so that when inserting and finding elements, every
position of the element in the hash table is considered. The technique used in this implementation to compute the probe sequence is quadratic probing. It uses a hash function of the form
h(k, i) = (h'(k) + c1 * i + c2 * i^2), where c1, and c2 are constants, and h'(k) is an auxiliary hash function (in this implementation, the division method was used). The positions probed depend
in a quadratic manner on the probe iteration i. This leads to an effect called secondary clustering, where slightly longer runs of occupied slots build up, increasing search time.

In this implementation, to ensure the uniform distribution of the elements to search in the table, the following scheme was used:
1.  Fill an array of a size two times larger than the elements to be inserted into the hash table.
2.  Select n random values from the array and put them into the table
3.  Select m random values from the array to search within the hash table, the probability of an element randomly selected being inside the table being 50% for very large values, since the
    array's size is two times the elements in the hash table.

Analysis:

The average effort for a successful search is close to 1, since the clusters (secondary clustering) are relatively mild with quadratic probing. On the other hand, in case of searching an
element not present in the table, every iteration has to be examined until a NULL is probed, which would be an absolute worst case for the insertion at that key. So searching for an element
not present in the table takes much more effort than searching a present one.
By increasing the fill factor, the average effort for both found and not found items increase, since more items need be examined in the process of searching.
For lower filling factors, the values of c1 and c2 affect heavily how the probe points are distributed, so even in an unsuccessful search, the maximum effort can be low. Even the
maximum efforts are miles lower than the worst case of the size of the table.

*/

int ops;

typedef struct node
{
    int id;
    char key[30];
} Node;

Node* table[TABLE_SIZE];

int c1 = 13, c2 = 23;

int random(int x, int y)
{
    srand(time(0));

    return (rand() % (y - x)) + x;
}

int hbeta(int k)
{
    return k % TABLE_SIZE;
}

int h(int k, int i)
{
    return (hbeta(k) + c1 * i + c2 * i * i) % TABLE_SIZE;
}

void initHashTable(Node* table[])
{
    int i = 0;
    for(; i < TABLE_SIZE; i++) table[i] = NULL;
}

Node* fillNode(int x)
{
    Node* New = (Node*) malloc(sizeof(Node));

    New->id = x;
    strcpy(New->key, "abc");

    return New;
}

Node* find_hashtable(Node* table[], int id)
{
    int i = 0;

    while(i != TABLE_SIZE)
    {
        int k = h(id, i);

        if(table[k])
        {
            ops++;
            if(table[k]->id == id)
            {
                return table[k];
            }

            else i++;
        }

        else return NULL;
    }

    return NULL;
}

void insert_hashtable(Node* table[], int id)
{
    int i = 0;

    while(i != TABLE_SIZE)
    {
        int k = h(id, i);

        if(!table[k])
        {
            table[k] = fillNode(id);
            return;
        }
        else i++;
    }
}

void print_A(Node* table[])
{
    for(int i = 0; i < TABLE_SIZE; i++)
    {
        if(table[i]) printf("%d: %d %s\n", i, table[i]->id, table[i]->key);
    }
    printf("\n");
}

void print_A(int a[], int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void insert_until_fillfactor(Node* table[], int m, float fill_factor)
{
    int max_effort_found = -1;
    int max_effort_not_found = -1;

    float avg_effort_found = 0;

    float avg_effort_not_found = 0;

    float total_found_rate = 0;
    int n = fill_factor * TABLE_SIZE;

    int poolA[2 * n] = {0};
    int tofindindexA[m] = {0};
    int toinsertindex[n] = {0};

    FillRandomArray(poolA, 2 * n, 0, 999999, true, UNSORTED);
    FillRandomArray(toinsertindex, n, 0, 2 * n - 1, true, UNSORTED);

    initHashTable(table);

    for(int i = 0; i < n; i++)
    {
        insert_hashtable(table, poolA[toinsertindex[i]]);
    }

    for(int k = 0; k < 5; k++)
    {
        int found = 0;
        int total_effort_found = 0;
        int total_effort_not_found = 0;

        FillRandomArray(tofindindexA, m, 0, 2 * n - 1, true, UNSORTED);

        for(int i = 0; i < m; i++)
        {
            //initialize operation counter
            ops = 0;

            //find code here
            if(find_hashtable(table, poolA[tofindindexA[i]]))
            {
                found++;

                total_effort_found += ops;

                if(ops > max_effort_found) max_effort_found = ops;
            }

            else
            {
                total_effort_not_found += ops;

                if(ops > max_effort_not_found) max_effort_not_found = ops;
            }
        }

        avg_effort_found += (float)total_effort_found / m;
        avg_effort_not_found += (float)total_effort_not_found / m;
    }

    avg_effort_found /= 5;
    avg_effort_not_found /= 5;

    printf("%.2f: Avg effort found: %.3f, Avg effort not found: %.3f, Max effort found: %d, Max effort not found: %d\n", fill_factor, avg_effort_found, avg_effort_not_found, max_effort_found, max_effort_not_found);
}

void test(int n, int m)
{
    int poolA[2 * n] = {0};
    int tofindindexA[m] = {0};
    int toinsertindex[n] = {0};

    FillRandomArray(poolA, 2 * n, 0, 99, true, UNSORTED);
    FillRandomArray(toinsertindex, n, 0, 2 * n - 1, true, UNSORTED);
    FillRandomArray(tofindindexA, m, 0, 2 * n - 1, true, UNSORTED);

    print_A(poolA, 2 * n);

    initHashTable(table);

    for(int i = 0; i < n; i++)
    {
        insert_hashtable(table, poolA[toinsertindex[i]]);
    }

    print_A(table);

    for(int i = 0; i < m; i++)
    {
        //find code here
        if(find_hashtable(table, poolA[tofindindexA[i]]))
        {
            printf("FOUND: %d\n", poolA[tofindindexA[i]]);
        }
        else printf("NOT FOUND: %d\n", poolA[tofindindexA[i]]);
    }
}

int main()
{
    insert_until_fillfactor(table, 3000, 0.80);
    insert_until_fillfactor(table, 3000, 0.85);
    insert_until_fillfactor(table, 3000, 0.90);
    insert_until_fillfactor(table, 3000, 0.95);
    insert_until_fillfactor(table, 3000, 0.99);
    //test(TABLE_SIZE, 5);

    return 0;
}
