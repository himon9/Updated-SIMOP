#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct concentration_factor node;

struct concentration_factor
{
    int index;
    int val;
    int pLeft, pRight;
    int iLower, iUpper; // lower index and upper index
    node *cLeft;
    node *cRight;
};

node *createNode()
{
    node *store = (node *)malloc(sizeof(node));
    int index;
    store->val = 0;
    store->pLeft = 0;
    store->pRight = 0;
    store->iLower = 0;
    store->iUpper = 0; // lower index and upper index
    store->cLeft = NULL;
    store->cRight = NULL;

    return store;
}

void samplePrep(node *parent)
{
    if (parent->cLeft == NULL)
    {
        int leftChildVal = (parent->val + parent->pLeft) / 2;
        if ((parent->val) % 2 == 0)
        {
            node *child = createNode();
            parent->cLeft = child;
            child->index = (parent->index) + 1;
            child->val = leftChildVal;
            child->pLeft = parent->pLeft;
            child->pRight = parent->val;

            samplePrep(child);
        }
        else
            return;
    }
    if (parent->cRight == NULL)
    {
        int rightChildVal = (parent->val + parent->pRight) / 2;
        if ((parent->val) % 2 == 0)
        {
            node *child = createNode();
            parent->cRight = child;
            child->index = (parent->index) + 1;
            child->val = rightChildVal;
            child->pLeft = parent->val;
            child->pRight = parent->pRight;

            samplePrep(child);
        }
        else
            return;
    }
}

void writeToFile(node *currNode, int lIndex, int uIndex)
{
    FILE *file = fopen("output2.txt", "a+");
    fprintf(file, "%10d   ", currNode->index);
    fprintf(file, "%10d   ", currNode->pLeft);
    fprintf(file, "%10d   ", lIndex);
    fprintf(file, "%10d   ", currNode->pRight);
    fprintf(file, "%10d   ", uIndex);
    fprintf(file, "%10d   \n", currNode->val);
    fclose(file);
}

int lowerIndex(node *currNode, node *root)
{
    int ans;
    if (currNode->pLeft == 0)
        return -1; // Standard index of buffer is -1 (assumed)
    else
    {
        node *temp = root;
        int key = currNode->pLeft;
        while (temp)
        {
            if (key > temp->val)
                temp = temp->cRight;
            else if (key < temp->val)
                temp = temp->cLeft;
            else if (key == temp->val)
            {
                ans = temp->index;
                break;
            }
        }
        return ans;
    }
}
int upperIndex(node *currNode, node *root, int solution)
{
    int ans;
    if (currNode->pRight == solution)
        return -2; // Standard index of solution is -2 (assumed)
    else
    {
        node *temp = root;
        int key = currNode->pRight;
        while (temp)
        {
            if (key > temp->val)
                temp = temp->cRight;
            else if (key < temp->val)
                temp = temp->cLeft;
            else if (key == temp->val)
            {
                ans = temp->index;
                break;
            }
        }
        return ans;
    }
}
void display(node *currNode, node *constRoot, node **list, int start, int tail, int solution)
{
    int lIndex = lowerIndex(currNode, constRoot);
    int uIndex = upperIndex(currNode, constRoot, solution);
   // printf("write to file calling \n");
    writeToFile(currNode, lIndex, uIndex); // Prining all the info to the file
    //printf("after writeToFile() : \n");
    start++;
    if (list[tail + 1] == NULL)
        list[++tail] = currNode->cLeft;
    if (list[tail + 1] == NULL)
        list[++tail] = currNode->cRight;

    if (start == tail)
    {
       // printf("display hitting base case  \n");
        return;
    }

    else
        display(list[start], constRoot, list, start, tail, solution);
}

int main()
{
    int steps;
    printf("Enter the number of steps : ");
    scanf("%d", &steps);

    int buffer = 0, solution = pow(2, steps);

    node *root = createNode();
    root->index = 0;
    root->val = (buffer + solution) / 2;
    root->pLeft = buffer;
    root->pRight = solution;
    root->iLower = -1;
    root->iUpper = -2;
    root->cLeft = NULL;
    root->cRight = NULL;

    samplePrep(root);
    node *list[solution]; // max size of the BFS array is 2^steps
    // Initialising list array elements with NULL
    for (int i = 0; i < solution; i++)
        list[i] = NULL;
    list[0] = root;
    int start = 0, tail = 0;

    // If segmentation error shows then comment the following FILE para
    FILE *file = fopen("output2.txt", "a+");
    fprintf(file, "%10s   ", "Index");
    fprintf(file, "%10s   ", "Lower_CF");
    fprintf(file, "%10s   ", "Lower_Index");
    fprintf(file, "%10s   ", "Upper_CF");
    fprintf(file, "%10s   ", "Upper_Index");
    fprintf(file, "%10s   \n", "Resultant_CF");
    fclose(file);
    //printf(" before display ok \n");
    display(root, root, list, start, tail, solution);

    printf("Data successfully stored in 'output2.txt' file\n");
    return 0;
}