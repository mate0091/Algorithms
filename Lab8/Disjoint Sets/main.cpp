#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define MAX_SIZE 100000

/**
COMMENTARY HERE
Disjoint sets

A disjoint-set data structure is a collection of disjoint sets, such that each set has a representative. This data structure has three main operations: make_set - creates the set with a given value
whose representative is itself, union - unites two sets, making the representative the same for them, find_set - returns a pointer to the representative element for the set. This implementation is
with disjoint-set forests, in which each node contains a member and a pointer to its parent. Each tree is a set. This implementation uses two heuristics to improve the running time of union and find_set,
union by rank and path compression. Using union by rank, we make the root with smaller rank point to the root with the larger rank, rank being the height of the tree. With path compression, we use it
during a find_set operation to make each node on the find path point to the root(representative). One application of disjoint sets is finding all the connected components of a graph.

Kruskal's algorithm is a straight forward minimum spanning tree construction algorithm. It works by finding at each step the lightest edge, that doesn't close a cycle, and adding the edge, as well
as the two vertexes incident to it into the MST. Using disjoint sets, if the edge connects two different trees, we add them into the same component, and unite them. Else if the two vertexes incident
on the edge to be added are already in the same set(component), we don't add the edge into the tree. This implementation uses quick sort as means to get an ordered list of the edges.

I used an adjacency list to represent the graph for which Kruskal's algorithm is called. To generate a connected graph with exactly 4 * n edges, I used the following strategy:
1.  Create a path that connects all the nodes in the graph, such that we start from a random node, and then take another vertex not yet in the path and add the edge connecting the two vertexes.
    Continue from the last vertex added, and stop after n - 1 iterations.
2.  Fill in the remaining edges by selecting an edge not yet in the graph, with random u, v vertexes, such that (u, v) will be the new edge.

Interpretation:
The two heuristics on disjoint sets provide a close to O(n) running time, this can be seen clearly on the chart, when running union and find_set together(both heuristics are in effect this way),
since the augmented data structures remain almost always optimal.

*/

int ops = 0;

Profiler p("Disjoint_sets");

typedef struct node
{
    int value;
    struct node* parent;
    int rank;
}
Node;

typedef struct edge
{
    int u, v, w;
} Edge;

Edge E[MAX_SIZE];
int edgecount = 0;

void initEdges()
{
    edgecount = 0;

    for(int i = 0; i < MAX_SIZE; i++)
    {
        E[i].u = 0;
        E[i].v = 0;
        E[i].w = 0;
    }
}

void print_edges()
{
    for(int i = 0; i < edgecount; i++)
    {
        if(E[i].w != 0)
        {
            printf("%d, %d = %d\n", E[i].u, E[i].v, E[i].w);
        }
    }

    printf("\n");
}

int check_edge(int u, int v)
{
    if(u == v) return 0;

    if (edgecount == 0) return 1;

    for(int i = 0; i < edgecount; i++)
    {
        if((E[i].u == u && E[i].v == v) || (E[i].u == v && E[i].v == u))
        {
            return 0;
        }
    }

    return 1;
}

void insert_to_edges(int u, int v, int weight)
{
    E[edgecount].u = u;
    E[edgecount].v = v;
    E[edgecount].w = weight;

    edgecount++;
}

void make_set(Node* x, int value)
{
    x->rank = 0;
    x->value = value;
    x->parent = x;

    ops += 3;
}

void swap_edges(Edge* a, Edge* b)
{
    Edge temp;

    temp.u = a->u;
    temp.v = a->v;
    temp.w = a->w;

    a->u = b->u;
    a->v = b->v;
    a->w = b->w;

    b->u = temp.u;
    b->v = temp.v;
    b->w = temp.w;
}

int part_rand(Edge a[], int l, int r)
{
    int pivot = rand() % (r - l) + l;

    swap_edges(&a[pivot], &a[r]); //std::swap(a[pivot], a[r]); //swap pivot with last element, now a[r] is the pivot

    int i = l - 1;

    for(int j = l; j < r; j++)
    {
        //ops++;

        if(a[j].w < a[r].w) //then swap
        {
            //ops += 3;
            i++;
            swap_edges(&a[i], &a[j]);
        }
    }
    swap_edges(&a[i + 1], &a[r]);

    //ops += 6;

    return i + 1;
}

void quicksort(Edge a[], int l, int r)
{
    if(l < r)
    {
        int pivotIndex = part_rand(a, l, r);

        quicksort(a, l, pivotIndex - 1);
        quicksort(a, pivotIndex + 1, r);
    }
}

Node* find_set(Node* x)
{
    ops++;
    if(x != x->parent)
    {
        ops++;
        x->parent = find_set(x->parent);
    }

    return x->parent;
}

void link(Node* x, Node* y)
{
    ops++;
    if(x->rank > y->rank)
    {
        ops++;
        y->parent = x;
    }

    else
    {
        ops++;
        x->parent = y;

        if(x->rank == y->rank)
        {
            ops++;
            y->rank++;
        }
    }
}

void union_set(Node* x, Node* y)
{
    link(find_set(x), find_set(y));
}

int random(int x, int y)
{
    return (rand() % (y - x)) + x;
}

void sets_demo()
{
    Node sets[10];

    for(int i = 0; i < 5; i++)
    {
        make_set(&sets[i], i);
    }

    for(int i = 0; i < 5; i++)
    {
        printf("%d %d\n", sets[i].value, find_set(&sets[i])->value);
    }

    printf("\nUniting 0 with 1:\n");
    union_set(&sets[0], &sets[1]);

    for(int i = 0; i < 5; i++)
    {
        printf("Set for representative %d: ", i);

        for(int j = 0; j < 5; j++)
        {
            if(find_set(&sets[j])->value == i)
            {
                printf("%d ", j);
            }
        }

        printf("\n");
    }

    printf("\nUniting 0 with 2:\n");
    union_set(&sets[0], &sets[2]);

    for(int i = 0; i < 5; i++)
    {
        printf("Set for representative %d: ", i);

        for(int j = 0; j < 5; j++)
        {
            if(find_set(&sets[j])->value == i)
            {
                printf("%d ", j);
            }
        }

        printf("\n");
    }

    printf("\nUniting 0 with 3:\n");
    union_set(&sets[0], &sets[3]);

    for(int i = 0; i < 5; i++)
    {
        printf("Set for representative %d: ", i);

        for(int j = 0; j < 5; j++)
        {
            if(find_set(&sets[j])->value == i)
            {
                printf("%d ", j);
            }
        }

        printf("\n");
    }

    printf("\nUniting 0 with 4:\n");
    union_set(&sets[0], &sets[4]);

    for(int i = 0; i < 5; i++)
    {
        printf("Set for representative %d: ", i);

        for(int j = 0; j < 5; j++)
        {
            if(find_set(&sets[j])->value == i)
            {
                printf("%d ", j);
            }
        }

        printf("\n");
    }
}

void generate_path(int n)
{
    int u, v;

    int succession[MAX_SIZE];
    int weights[n];

    FillRandomArray(succession, n, 0, n - 1, true, UNSORTED);
    FillRandomArray(weights, n, 1, 99, false, UNSORTED);

    u = succession[0];

    for(int i = 1; i < n; i++)
    {
        v = succession[i];

        int weight = weights[i];

        insert_to_edges(u, v, weight);

        u = v; //swap vertex with last generated
    }
}

void generate_graph(int n, int edge_max)
{
    generate_path(n); //generated a connected graph with n-1 edges

    while(edgecount != edge_max)
    {
        int u, v;

        v = random(0, n);
        u = random(0, n);

        while(check_edge(u, v) == 0)
        {
            //srand(time(0));

            v = random(0, n);
            u = random(0, n);
        }

        int weight = random(1, 99);

        insert_to_edges(u, v, weight);
    }
}

void Kruskal(int n, bool flag)
{
    Edge A[MAX_SIZE] = {0};
    int asize = 0;

    Node nodes[n];

    for(int i = 0; i < n; i++)
    {
        make_set(&nodes[i], i);
    }

    //sort edges
    quicksort(E, 0, edgecount - 1);

    for(int i = 0; i < edgecount; i++)
    {
        int u = E[i].u;
        int v = E[i].v;
        int w = E[i].w;

        if(find_set(&nodes[u])->value != find_set(&nodes[v])->value)
        {
            if(flag)
            {
                A[asize].u = u;
                A[asize].v = v;
                A[asize].w = w;
                asize++;
            }

            union_set(&nodes[u], &nodes[v]);
        }
    }

    if(flag)
    {
        printf("Kruskal MST:\n");

        for(int i = 0; i < asize; i++)
        {
            printf("%d, %d = %d\n", A[i].u, A[i].v, A[i].w);
        }
    }
}

void eval_kruskal()
{
    for(int n = 100; n <= 10000; n+= 100)
    {
        initEdges();

        ops = 0;
        generate_graph(n, 4 * n);

        Kruskal(n, 0);

        p.countOperation("Kruskal_ops", n, ops);
    }
}

void kruskal_demo()
{
    initEdges();
    generate_graph(5, 10);

    print_edges();

    Kruskal(5, 1);
}

void demo()
{
    sets_demo();
    printf("\nGraph:\n");
    kruskal_demo();
}

int main()
{
    srand(time(0));

    demo();
    eval_kruskal();
    p.showReport();


    return 0;
}
