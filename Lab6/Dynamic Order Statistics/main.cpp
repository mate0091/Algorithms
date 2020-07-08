#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define MAX_SIZE 100000

/**
COMMENTARY HERE
This is an implementation of an Order statistic tree and its main operations.
An order - statistic tree (OS tree) is a red - black tree (here binary tree), in which every node has one additional information, the size of the subtree rooted at it. An ith order statistic of a set
of n elements is the element in the set with the ith smallest key. In case of OS trees, the retrieval of the ith smallest element can be done in O(log(n)) time, but in this case, since the OS tree
is an arbitrary binary tree with no balancing operations, this operation will be done in O(h) time. Two main procedures use the size of the subtree rooted at an element. The one implemented here is
OS_Select, which selects the ith smallest element from the tree. It works as follows: First we compute the rank of the subtree, which is its place in an inorder walk. Its rank will be left->size + 1,
since there are left->size number of nodes before it in the inorder walk, and our element would be the next. If our element's rank is i, we are done, else we need to check in which subtree we
have to continue searching the element with rank i. If our computed rank is less then what we are searching for, we search the ith element in the left subtree, if it is greater, we search on the
right subtree the rank - i th element. This algorithm works in O(h) (h - height of the tree), since the height is not kept under control by balancing. All in all, OS_Select does not require the
tree to be balanced, since we only need to calculate an element's rank, which does not depend on the balance factor. Only the running time of the algorithm is modified, when not implemented on
balanced trees. This algorithm is similar to the quickselect algorithm studied earlier, but provides a massive improvement in worst case behavior just by adding one field of information, because
quickselect has a complexity of O(n^2) at worst, and OS_Select performs in O(h), log(n) <= h <= n.
The other implemented algorithm here is OS_Delete. This is essentially the standard delete operation for binary trees. The deletion is done by replacing the element with the minimum from its
right subtree, so that the height of the tree does not change. This node is guaranteed to have either one child (right) or be a leaf. After this, we recalculate the size of the subtree where
the operation was done. This is done at the last segment of the algorithm, after processing each node involved in the deletion recursively, by using the formula size = left->size + right->size + 1.
This algorithm runs in O(h) time also. There is no additional traversal to recalculate the amount of elements after a deletion, the calculation is done in a postorder manner in the deletion algorithm
itself, so the complexity does not change.
The creation of the perfectly balanced tree is done using a divide-and-conquer approach: create a new node from the middle element of the array, then process the left side of the array for the left
child and the right side of the array for the right child, then calculate the size of the subtree rooted at node recursively.

The graphs show very clearly the linearity of the OS_Select and OS_Delete operations, since they both run in O(h) time, with no balancing. There is one assignment and one comparison at each iteration
of the OS_Select, and for the OS_Delete, there is one operation at every call of the minimum selection (O(h)), one operation when replacing the element with the minimum, one operation when fixing the
sizes at the leaves, one operation when recurring, and one operation when fixing the sizes for the subtree which was modified. In conclusion, they both run in linear time O(h).

*/

int sel_ops = 0;
int del_ops = 0;

Profiler p("Dynamic_Order_Statistics"); //static constructor wtf

typedef struct node
{
    int id;
    struct node* left;
    struct node* right;
    int size;
} Node;

int calc_size(Node* root)
{
    if(!root) return 0;

    return 1 + calc_size(root->left) + calc_size(root->right);
}

Node* build_PBT(int a[], int l, int r)
{
    if (l > r) return NULL;

    Node* New = (Node*)malloc(sizeof(Node));

    int mid = (l + r) / 2;

    New->id = a[mid];
    New->left = build_PBT(a, l, mid - 1);
    New->right = build_PBT(a, mid + 1, r);
    New->size = calc_size(New);

    return New;
}

void inorder(Node* root, int level)
{
    if(!root) return;

    inorder(root->left, level + 1);

    for(int i = 0; i < level; i++)
    {
        printf("    ");
    }

    printf("%d/%d\n", root->id, root->size);

    inorder(root->right, level + 1);
}

Node* OS_Select(Node* root, int i)
{
    int k;

    if(!root->left) k = 0;
    else k = root->left->size;

    sel_ops += 2;

    if(i == k + 1) return root;
    else if (i < k + 1) return OS_Select(root->left, i);
    else return OS_Select(root->right, i - k - 1);
}

Node* BST_min(Node* root)
{
    if(!root) return NULL;

    del_ops++;

    if(root->left) return BST_min(root->left);
    else return root;
}

Node* OS_Delete(Node* root, int i)
{
    if(!root);

    else if(i < root->id)
    {
        root->left = OS_Delete(root->left, i);
    }

    else if(i > root->id)
    {
        root->right = OS_Delete(root->right, i);
    }

    else // found element, replace it with the rightmost element of the left subtree
    {
        if(root->right && root->left)
        {
            Node* min = BST_min(root->right);

            del_ops++;

            root->id = min->id;
            root->right = OS_Delete(root->right, min->id);
        }

        else //element has one or zero children, can delete directly
        {
            Node* toDel = root;

            del_ops++;

            if(root->left)
            {
                root = root->left;
                root->size = 1;
            }

            else if(root->right)
            {
                root = root->right;
                root->size = 1;
            }

            else
            {
                root = NULL;
            }

            free(toDel);
        }
    }

    del_ops ++;

    if(root)
    {
        if(!root->left && !root->right) root->size = 1;
        else if(root->left && !root->right) root->size = root->left->size + 1;
        else if(!root->left && root->right) root->size = root->right->size + 1;
        else if(root->left && root->right) root->size = root->left->size + root->right->size + 1;

        del_ops++;
    }

    return root;
}

int random(int x, int y)
{
    srand(time(0));

    return (rand() % (y - x)) + x;
}

void test()
{
    Node* root = NULL;

    int arr[MAX_SIZE];
    int n = 12;

    FillRandomArray(arr, 25, 0, 99, true, ASCENDING);

    root = build_PBT(arr, 0, 23);

    inorder(root, 0);

    int i;

    for(int j = 0; j < 3; j++)
    {
        printf("i: ");
        scanf("%d", &i);

        root = OS_Delete(root, i);

        inorder(root, 0);

        printf("\n\n");
    }

    printf("\n\n");

    for(int j = 0; j < 3; j++)
    {
        printf("i: ");
        scanf("%d", &i);

        Node* ith = OS_Select(root, i);

        printf("%d-th: %d/%d\n\n", i, ith->id, ith->size);
    }
}

void eval()
{
    int arr[MAX_SIZE];

    for(int k = 0; k < 5; k++)
    {
        for(int n = 100; n <= 10000; n += 100)
        {
            Node* root = NULL;

            FillRandomArray(arr, n + 1, 0, 99999, true, ASCENDING);

            root = build_PBT(arr, 0, n - 1);

            sel_ops = del_ops = 0;

            for(int i = 0; i < n; i++)
            {
                int index = random(1, n - i + 1);
                Node* del = OS_Select(root, index);

                root = OS_Delete(root, del->id);
            }

            p.countOperation("OS_Select", n, sel_ops);
            p.countOperation("OS_Delete", n, del_ops);
        }
    }

    p.divideValues("OS_Select", 5);
    p.divideValues("OS_Delete", 5);

    p.createGroup("OS_Operations", "OS_Select", "OS_Delete");
    p.showReport();
}

int main()
{
    test();
    eval();

    return 0;
}
