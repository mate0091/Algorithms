#include "SLList.hpp"

//implementing the functions
void initList(SLList *List)
{
    List->first = NULL;
    List->last = NULL;
}

int contentAt(SLList* List, int index)
{
    Node* walker = List->first;

    for(int i = 0; i < index; i++)
    {
        if(!walker) return -1;
        walker = walker->next;
    }

    return walker->content;
}

Node* nodeAT(SLList* List, int index)
{
    Node* walker = List->first;

    for(int i = 0; i < index; i++)
    {
        if(!walker) return NULL;
        walker = walker->next;
    }

    return walker;
}

void insertAtRear(SLList *List, int x) //insert to tail
{
    if(List)
    {
        Node *New = (Node*) malloc(sizeof(Node));

        New->next = NULL;
        New->content = x;

        if(List->first == NULL) //list is empty
        {
            List->first = New;
        }
        else
        {
            List->last->next = New;
        }

        List->last = New;
    }
}

void insertAtFront(SLList *List, int x)
{
    if(List)
    {
        Node *New = (Node*) malloc(sizeof(Node)); // allocate space for new node

        New->content = x; //make connections
        New->next = List->first;

        if(List->first == NULL) //empty list
        {
            List->last = New;
        }

        List->first = New;
    }
}

void insertAt(SLList* List, int index, int content)
{
    Node *temp = List->first, *link = NULL;

    if(List)
    {
        Node* New = (Node*) malloc(sizeof(Node));

        New->content = content;

        int i = 0;
        while(temp != NULL && i != index)// search for the id
        {
            link = temp;
            temp = temp->next;
            i++;
        }

        if(temp)
        {
            if(temp == List->first)
            {
                New->next = List->first;
                List->first = New;
            }

            else
            {
                link->next = New;
                New->next = temp;
            }
        }

        else if(link)
        {
            free(New);
            insertAtRear(List, content);
        }
    }
}

void deleteAtFront(SLList *List) //delete the first
{
    Node *temp = NULL;

    if(List)
    {
        if(List->first != NULL) //List is not empty
        {
            temp = List->first;
            List->first = List->first->next;
            free(temp);
        }
        else //list is empty
        {
            List->last = NULL;
        }
    }
}

void deleteKey(SLList *List, int index)
{
    Node *temp = List->first, *link = NULL; // temp is the node to be deleted, and link is the node before the searched node

    if(List)
    {
        int i = 0;
        while(temp != NULL && i != index)// search for the id
        {
            link = temp;
            temp = temp->next;
            i++;
        }

        if(temp != NULL) //found the node
        {
            if(temp != List->first) //found node is not the first
            {
                link->next = temp->next;
                if(temp == List->last) //found node is last
                {
                    List->last = link;
                }
                free(temp);
            }

            else //is the first or is the only element in list
            {
                List->first = List->first->next;
                free(temp);
                if(List->first == NULL) List->last == NULL;
            }
        }
    }
}

void swapNodes(SLList* List, int x, int y)
{
    int tempA = contentAt(List, x);
    int tempB = contentAt(List, y);

    if(tempA != -1 && tempB != -1 && tempA != tempB)
    {
        deleteKey(List, x);
        insertAt(List, x, tempB);
        deleteKey(List, y);
        insertAt(List, y, tempA);
    }
}

void printList(SLList *List)
{
    Node *walker = List->first;

    for(; walker != NULL; walker = walker->next)
    {
        printf("%d ", walker->content);
    }

    printf("\n");
}
