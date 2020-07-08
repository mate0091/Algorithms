#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#include "SLList.hpp"

#define MAX_SIZE 6000

/**
COMMENTARY HERE

BFS

The breadth - first search is one of the simplest algorithms for searching in a graph. Given a graph, the algorithm explores its edges to discover all the reachable vertexes from a source vertex.
It computes the shortest path between a source and all reachable vertexes and constructs a breadth-first tree. It is called breadth-first because it expands the frontier edges between discovered
and undiscovered vertexes by first discovering the vertexes at level k before discovering any vertex at distance k + 1. To keep track of progress, all vertexes receive colors: white if it is unvisited,
gray is it is under discovery, and black if it is visited. All vertexes start out white. A vertex is discovered the first time it is encountered during a search, at which it becomes gray, and later,
black. We use a queue to keep track of the vertexes currently being under visitation. BFS also constructs a breadth-first tree, which starts from a given source root.
Whenever the algorithm discovers a vertex v while scanning the adjacency list of an already visited vertex u, the edge (u, v) and vertex v is added to the tree. Then u will be the parent of v.
The algorithm will end up in a constructed parent vector which we can use to pretty-print the tree.

Analysis:
The BFS algorithm works in O(|V| + |E|) complexity, because the algorithm uses the enqueue and dequeue operations for each vertex, which takes O(|V|) time in total, and the algorithm scans the
adjacency list of each vertex at most once, and the sum of all adjacency lists is |E|. The running time of the algorithm is linear, and the chart shows this very clearly.


*/

Profiler p("BFS");

int ops = 0;

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
        if((E[i].u == u && E[i].v == v) || (E[i].u == v && E[i].v == u))
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

void adjacent_vertexes(int v, int a[], int* n)
{
    (*n) = 0;

    for(int i = 0; i < MAX_SIZE; i++)
    {
        a[i] = 0;
    }

    for(int i = 0; i < edgecount; i++)
    {
        ops++;

        if(E[i].u == v)
        {
            a[(*n)] = E[i].v;

            (*n)++;

            ops++;
        }

        else if(E[i].v == v)
        {
            a[(*n)] = E[i].u;

            (*n)++;

            ops++;
        }
    }
}

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

void BFS(int source, int n, int p[], int visited[]) //n = vertex count
{
    for(int i = 0; i < MAX_SIZE; i++)
    {
        p[i] = -2;
    }
    //insert at rear, delete at front
    SLList Q;
    initList(&Q);

    visited[source] = 1;

    p[source] = -1;

    insertAtRear(&Q, source);

    ops ++;

    //add source as parent to tree

    while(Q.first)
    {
        int v = Q.first->content;

        deleteAtFront(&Q);

        ops++;

        //for each frontier edge of v
        int u[MAX_SIZE];
        int ucount = 0;

        adjacent_vertexes(v, u, &ucount);

        for(int i = 0; i < ucount; i++)
        {
            ops++;

            if(visited[u[i]] == 0)
            {
                //visit it
                visited[u[i]] = 1;

                p[u[i]] = v;

                insertAtRear(&Q, u[i]);

                ops ++;
            }
        }
    }
}

void BFSwrapper(int n, int edgec, bool flag) //n - vertex count
{
    int visited[MAX_SIZE] = {0};

    generate_graph(n, edgec);

    if(flag)
    {
        printf("Graph:\n");
        print_edges();
        printf("\nBFS forest:\n");
    }

    for(int i = 0; i < n; i++)
    {
        if(visited[i] == 0)
        {
            //BFS, construct tree, whatever else...
            int parent[MAX_SIZE];

            BFS(i, n, parent, visited);

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

void demo()
{
    BFSwrapper(32, 20, true);
}

void eval_BFS1()
{
    for(int e = 1000; e <= 4500; e += 100)
    {
        ops = 0;

        BFSwrapper(100, e, false);

        p.countOperation("varying_edges", e, ops);
    }

    p.createGroup("edges", "varying_edges");
}

void eval_BFS2()
{
    for(int v = 100; v <= 200; v += 10)
    {
        ops = 0;

        BFSwrapper(v, 4500, false);

        p.countOperation("varying_vertexes", v, ops);
    }

    p.createGroup("vertexes", "varying_vertexes");
}

int main()
{
    srand(time(0));

    demo();
    eval_BFS1();

    eval_BFS2();

    p.showReport();

    return 0;
}
