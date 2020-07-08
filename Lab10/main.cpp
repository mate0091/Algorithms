#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#include "SLList.hpp"

#define MAX_SIZE 10000

/**
COMMENTARY HERE


*/

Profiler p("BFS");

int ops = 0;

//tree handling
typedef struct node1
{
    int value;
    struct node1* children[MAX_SIZE];
    int childcnt;
}Node1;

typedef struct node2
{
    int value;
    struct node2* left_child;
    struct node2* right_sibling;
}Node2;

//graph handling
typedef struct edge
{
    int u, v;
} Edge;

Edge E[MAX_SIZE];
int edgecount = 0;

int random(int x, int y)
{
    return (rand() % (y - x)) + x;
}

void initEdges()
{
    edgecount = 0;

    for(int i = 0; i < MAX_SIZE; i++)
    {
        E[i].u = 0;
        E[i].v = 0;
    }
}

void print_edges()
{
    for(int i = 0; i < edgecount; i++)
    {
        if(!(E[i].u == 0 && E[i].v == 0))
        {
            printf("%d, %d\n", E[i].u, E[i].v);
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
        if(E[i].u == u && E[i].v == v)
        {
            return 0;
        }
    }

    return 1;
}

void insert_to_edges(int u, int v)
{
    E[edgecount].u = u;
    E[edgecount].v = v;

    edgecount++;
}

void generate_graph(int n, int edge_max)
{
    initEdges();

    while(edgecount != edge_max)
    {
        int u, v;

        do
        {
            v = random(0, n);
            u = random(0, n);
        }
        while(check_edge(u, v) == 0);

        insert_to_edges(u, v);
    }
}

void adjacent_vertexes(int u, int a[], int* n)
{
    (*n) = 0;

    for(int i = 0; i < MAX_SIZE; i++)
    {
        a[i] = 0;
    }

    for(int i = 0; i < edgecount; i++)
    {
        ops++;

        if(E[i].u == u)
        {
            a[(*n)] = E[i].v;

            (*n)++;

            ops++;
        }
    }
}

Node1* createNode1(int id)
{
    Node1* New = (Node1*)malloc(sizeof(Node1));

    New->childcnt = 0;
    New->value = id;

    for(int i = 0; i < MAX_SIZE; i++)
    {
        New->children[i] = NULL;
    }

    return New;
}

Node2* createNode2(int id)
{
    Node2* New = (Node2*)malloc(sizeof(Node2));

    New->value = id;
    New->left_child = NULL;
    New->right_sibling = NULL;

    return New;
}

Node1* build_MWT(int parent[], int n)
{
    Node1* nodes[n];
    Node1* root = NULL;

    for(int i = 0; i < n; i++)
    {
        nodes[i] = createNode1(i);
    }

    for(int i = 0; i < n; i++)
    {
        if(parent[i] >= 0)
        {
            nodes[parent[i]]->children[nodes[parent[i]]->childcnt++] = nodes[i];
        }

        else if(parent[i] == -1)
        {
            root = nodes[i];
        }
    }

    return root;
}

Node2* transform_tree(Node1* root)
{
    if(!root) return NULL;

    Node2* root2 = createNode2(root->value);

    root2->left_child = transform_tree(root->children[0]);

    Node2* current = root2->left_child;

    for(int i = 1; i < root->childcnt; i++)
    {
        current->right_sibling = transform_tree(root->children[i]);
        current = current->right_sibling;
    }

    return root2;
}

void print_tree(Node2* root, int level)
{
    if(root == NULL) return;

    for(int i = 0; i < level; i++) printf("  ");
    printf("%d\n", root->value);

    print_tree(root->left_child, level + 2);
    print_tree(root->right_sibling, level);
}

int t;

int d[MAX_SIZE] = {0};
int f[MAX_SIZE] = {0};

void DFS(int source, int parent[], int visited[]) //n = vertex count
{
    t++;

    visited[source] = 1;

    d[source] = t;

    //for each frontier edge of v
    int u[MAX_SIZE];
    int ucount = 0;

    adjacent_vertexes(source, u, &ucount);

    for(int i = 0; i < ucount; i++)
    {
        if(visited[u[i]] == 0)
        {
            parent[u[i]] = source;

            DFS(u[i], parent, visited);
        }
    }

    f[source] = ++t;
}

void DFSwrapper(int n, int edgec, bool flag) //n - vertex count
{
    t = 0;

    int visited[MAX_SIZE] = {0};

    generate_graph(n, edgec);

    if(flag)
    {
        printf("Graph:\n");
        print_edges();
        printf("\nDFS forest:\n");
    }

    for(int i = 0; i < n; i++)
    {
        if(visited[i] == 0)
        {
            //BFS, construct tree, whatever else...
            int parent[MAX_SIZE];

            for(int j = 0; j < MAX_SIZE; j++)
            {
                parent[j] = -2;
            }

            parent[i] = -1;

            DFS(i, parent, visited);

            if(flag)
            {
                Node1* root = build_MWT(parent, n);
                Node2* root2 = transform_tree(root);

                print_tree(root2, 0);

                printf("\n");
            }
        }
    }
}

void print_A(int a[], int n)
{
    for(int i = 0; i < n; i++) printf("%d ", a[i]);

    printf("\n");
}

void topological_sort()
{
    //compute final times of all the nodes with DFS

    //as each vertex is finished, insert onto the top of a linked list

    //return linked list of vertexes
}

void demo()
{
    DFSwrapper(32, 30, true);


    printf("Discovery time:\n");
    print_A(d, 32);

    printf("Final time:\n");
    print_A(f, 32);

}

int main()
{
    srand(time(0));
    demo();

    return 0;
}
