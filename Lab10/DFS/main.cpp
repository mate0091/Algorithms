#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#include "SLList.hpp"

#define MAX_SIZE 10000

/**
COMMENTARY HERE

The DFS (Depth - first search) is a basic graph traversal algorithm and is a building block of many other graph algorithms. DFS explores edges out of the most recently discovered vertex that has
still unexplored edges incident on it. Once all the edges have been discovered, the search backtracks to find the first already discovered vertex that has unexplored edges incident on it and
continues from there. If any undiscovered vertexes remain, the DFS executes this routine for the first undiscovered vertex. DFS also creates a depth - first forest of the graph by setting parent
indexes for the discovered vertexes. The edges in the depth - first forest are called tree edges. DFS colors vertexes just like BFS does, each vertex being initially white when undiscovered, gray
when being under discovery and black when it was discovered. Besides creating depth - first trees, DFS also computes the discovery time and final time for each vertex. Discovery time is assigned to
a vertex at the time when it has been discovered (changed its color from white to gray), and final time is assigned when all of the adjacent nodes of the vertex have been examined (changed its color
from gray to black). These timestamps provide useful information on the behavior of DFS on the given graph.
DFS can also classify the edges into 4 categories:
-tree edges - edges which are in the depth - first forest
-back edges - edges (u, v) connecting vertex u to an ancestor v in a depth - first tree
-forward edges - edges (u, v) connecting vertex u to a descendant v in a depth - first tree
-cross edges - all other edges

Topological sort can be performed on any directed acyclic graph (dag). Topologically sorting a dag means to sort the vertexes of the graph such that if (u, v) is an edge, the u is before v in the
linear ordering. If the graph contains cycles, then it is impossible to topologically sort it. A simple algorithm to topologically sort a dag is to compute the finishing times for all vertexes using
DFS, and as each vertex is finished, push it onto the top of a stack. Lastly print the vertexes in a top - down manner.

Analysis:
DFS runs in linear time, having a stable O(V + E) complexity, because it has to examine the adjacency list of each vertex from the graph exactly once, and the sums of all edges in adjacency lists
is the number of edges. The chart shows very well the linearity of the running time of the algorithm.


*/

Profiler p("DFS");

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

SLList topSort;

void DFS(int source, int parent[], int visited[], bool topS) //n = vertex count
{
    t++;

    visited[source] = 1;

    d[source] = t;

    //for each frontier edge of v
    int u[MAX_SIZE];
    int ucount = 0;

    ops += 3;

    adjacent_vertexes(source, u, &ucount);

    for(int i = 0; i < ucount; i++)
    {
        ops++;
        if(visited[u[i]] == 0)
        {
            parent[u[i]] = source;

            DFS(u[i], parent, visited, topS);
        }
    }

    f[source] = ++t;

    if(topS) insertAtFront(&topSort, source);
}

void DFSwrapper(int n, int edgec, bool flag, bool topS) //n - vertex count
{
    t = 0;

    int visited[MAX_SIZE] = {0};

    if(!topS) generate_graph(n, edgec);

    for(int i = 0; i < MAX_SIZE; i++)
    {
        d[i] = 0;
        f[i] = 0;
    }

    if(flag)
    {
        printf("Graph:\n");
        print_edges();
        printf("\nDFS forest:\n");
    }

    ops++;

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

            DFS(i, parent, visited, topS);

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
    //hard coded graph
    initEdges();

    insert_to_edges(0, 1);
    insert_to_edges(0, 7);
    insert_to_edges(1, 2);
    insert_to_edges(1, 7);
    insert_to_edges(2, 5);
    insert_to_edges(3, 2);
    insert_to_edges(3, 4);
    insert_to_edges(4, 5);
    insert_to_edges(6, 7);

    initList(&topSort);

    //compute final times of all the nodes with DFS while pushing to stack when assigning finishing time
    DFSwrapper(9, 9, true, true);

    printf("Graph topologically sorted:\n");
    printList(&topSort);
}

SLList tarjan_stack;

int min(int a, int b)
{
    return (a >= b)? a: b;
}

void tarjan_dfs(int source, int low_link[], int on_stack[])
{
    d[source] = t;
    low_link[source] = t;

    insertAtFront(&tarjan_stack, source);
    on_stack[source] = 1;

    t++;

    int u[MAX_SIZE];
    int ucount = 0;

    adjacent_vertexes(source, u, &ucount);

    for(int i = 0; i < ucount; i++)
    {
        if(d[u[i]] == -1)
        {
            tarjan_dfs(u[i], low_link, on_stack);
            low_link[source] = min(low_link[source], low_link[u[i]]);
        }

        else if(on_stack[u[i]] == 1)
        {
            low_link[source] = min(low_link[source], d[u[i]]);
        }
    }

    if(low_link[source] == d[source])
    {
        int popped;

        do
        {
            popped = tarjan_stack.first->content;
            deleteAtFront(&tarjan_stack);
            on_stack[popped] = 0;

            printf("%d ", popped);
        }
        while(popped != source);

        printf("\n");
    }
}

void hardcode_graph()
{
    initEdges();

    insert_to_edges(0, 1);
    insert_to_edges(1, 2);
    insert_to_edges(1, 4);
    insert_to_edges(1, 5);
    insert_to_edges(2, 3);
    insert_to_edges(3, 4);
    insert_to_edges(4, 3);
    insert_to_edges(5, 0);
    insert_to_edges(5, 4);
}

void tarjan_scc(int v)
{
    hardcode_graph();

    initList(&tarjan_stack);

    int low_link[MAX_SIZE] = {0};
    int on_stack[MAX_SIZE] = {0};

    t = 0;

    for(int i = 0; i < v; i++)
    {
        d[i] = -1;
    }

    for(int i = 0; i < v; i++)
    {
        if(d[i] == -1)
        {
            tarjan_dfs(i, low_link, on_stack);
        }
    }
}

void demo()
{
    DFSwrapper(32, 30, true, false);

    printf("Discovery time:\n");
    print_A(d, 32);

    printf("Final time:\n");
    print_A(f, 32);
}

void DFS_for_eval(int source, int n, int p[], int visited[]) //n = vertex count
{
    for(int i = 0; i < MAX_SIZE; i++)
    {
        p[i] = -2;
    }

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

                insertAtFront(&Q, u[i]);

                ops ++;
            }
        }
    }
}

void DFSwrapper_for_eval(int n, int edgec) //n - vertex count
{
    int visited[MAX_SIZE] = {0};

    generate_graph(n, edgec);

    for(int i = 0; i < n; i++)
    {
        if(visited[i] == 0)
        {
            //BFS, construct tree, whatever else...
            int parent[MAX_SIZE];

            DFS_for_eval(i, n, parent, visited);
        }
    }
}

void eval_DFS1()
{
    for(int e = 1000; e <= 4500; e += 100)
    {
        ops = 0;

        DFSwrapper_for_eval(100, e);

        p.countOperation("varying_edges", e, ops);
    }

    p.createGroup("edges", "varying_edges");
}

void eval_DFS2()
{
    for(int v = 100; v <= 200; v += 10)
    {
        ops = 0;

        DFSwrapper_for_eval(v, 4500);

        p.countOperation("varying_vertexes", v, ops);
    }

    p.createGroup("vertexes", "varying_vertexes");
}

int main()
{
    srand(time(0));
    demo();
    topological_sort();
    eval_DFS1();
    eval_DFS2();

    printf("\n");

    tarjan_scc(6);

    p.showReport();

    return 0;
}
