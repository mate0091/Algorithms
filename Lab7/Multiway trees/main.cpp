#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 13

/**
COMMENTARY HERE

This is an implementation of a multi-way tree.
Given the parent vector, two transformations are needed to reach a binary tree representation, where the left child of a node is its first child, and the right child is a sibling of his.
Two data structures are used for this implementation. The first data structure is for the intermediary tree, and it contains a value, a static array of pointers to the children, and the
child count as a helper field when making the links (next child is inserted at child count + 1). The second data structure is a binary tree with every node containing a value, a pointer to left child
and a pointer to a sibling (right child).
The first algorithm transforms the parent vector into such a tree, where a node contains its children too as an array of pointers to them. This means O(n) space for each node.
This algorithm runs in O(n) complexity, since each of the indexes of the parent vector have to be put somewhere in the tree, and this takes O(1) time (put it as a child of the value
in parent vector). An additional space of O(n) is used here as an array of nodes to determine the links, because we don't know which the root is and what belongs where. Finally we get
the root of the new tree constructed.
The second form of the tree is much more optimal than this, because there is no need to allocate O(n) additional space for the children of every node, instead the children are handled dynamically.
This is done in the following way recursively:
1. Build each left subtree recursively and continue from the first child.
2. Build the right siblings recursively by traversing each of the children and continuing from the right sibling of the newly built node.
This algorithm uses no additional space, and runs in O(n) complexity, since each of the nodes is visited once.
By traversing the tree built by this algorithm in a preorder manner, increasing the amount of spaces we put before a value only when we traverse the left child, we get a pretty-printed multi-way tree
in a binary tree representation.
*/

typedef struct node
{
    int value;
    struct node* children[MAX_SIZE];
    int childcnt;
}Node;

typedef struct node2
{
    int value;
    struct node2* left_child;
    struct node2* right_sibling;
}Node2;

Node* createNode(int id)
{
    Node* New = (Node*)malloc(sizeof(Node));

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

Node* build_MWT(int parent[], int n)
{
    Node* nodes[n];
    Node* root = NULL;

    for(int i = 0; i < n; i++)
    {
        nodes[i] = createNode(i);
    }

    for(int i = 0; i < n; i++)
    {
        if(parent[i] != -1)
        {
            nodes[parent[i]]->children[nodes[parent[i]]->childcnt++] = nodes[i];
        }

        else
        {
            root = nodes[i];
        }
    }

    return root;
}

Node2* transform_tree(Node* root)
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

void print_tree(Node* root, int level)
{
    for(int i = 0; i < level; i++)
    {
        printf("    ");
    }

    printf("%d\n", root->value);

    for(int i = 0; i < root->childcnt; i++)
    {
        print_tree(root->children[i], level + 1);
    }
}

void print_tree(Node2* root, int level)
{
    if(root == NULL) return;

    for(int i = 0; i < level; i++) printf("  ");
    printf("%d\n", root->value);

    print_tree(root->left_child, level + 2);
    print_tree(root->right_sibling, level);
}

int main()
{
    int a[MAX_SIZE] = {5, 0, 6, 0, 5, -1, 5, 6, 4, 0, 0, 0, 0};

    Node* root = NULL;

    root = build_MWT(a, MAX_SIZE);

    Node2* root2 = NULL;

    printf("The parent vector:\n");

    for(int i = 0; i < MAX_SIZE; i++)
        printf("%d ", a[i]);

    printf("\nAfter first transformation:\n");
    print_tree(root, 0);

    root2 = transform_tree(root);

    printf("\ntAfter second transformation:\n");

    print_tree(root2, 0);
    return 0;
}
