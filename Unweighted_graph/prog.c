#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct concentration_factor node;

struct concentration_factor
{
    int index;
    int val;
    int pLeftVal, pRightVal;
    int iLower, iUpper; // lower index and upper index
    int inDeg, outDeg;
    node *cLeft, *cRight;
    node *pLeft, *pRight;
};

node *createNode()
{
    node *store = (node *)malloc(sizeof(node));
    store->index = 0;
    store->val = 0;
    store->pLeftVal = 0;
    store->pRightVal = 0;
    store->iLower = 0;
    store->iUpper = 0; // lower index and upper index
    store->inDeg = 0;
    store->outDeg = 0;
    store->cLeft = NULL;
    store->cRight = NULL;
    store->pLeft = NULL;
    store->pRight = NULL;

    return store;
}

void samplePrep(node *parent, int level, int steps)
{
    if (level == steps)
    {

        /*printf("Base case hit \n");
        printf("level base hit value : %d\n",level);*/
        return;
    }

    if (parent->cLeft == NULL)
    {
        int leftChildVal = (parent->val + parent->pLeftVal) / 2;
        if ((parent->val) % 2 == 0)
        {
            node *child = createNode();
            parent->cLeft = child;
            child->index = (parent->index) + 1;
            child->val = leftChildVal;
            child->pLeftVal = parent->pLeftVal;
            child->pRightVal = parent->val;
            child->iLower = parent->iLower;
            child->iUpper = parent->index;
            child->pLeft = parent->pLeft;
            child->pRight = parent;

            samplePrep(child,level+1,steps);
        }
        else
            return;
    }
    if (parent->cRight == NULL)
    {
        int rightChildVal = (parent->val + parent->pRightVal) / 2;
        if ((parent->val) % 2 == 0)
        {
            node *child = createNode();
            parent->cRight = child;
            child->index = (parent->index) + 1;
            child->val = rightChildVal;
            child->pLeftVal = parent->val;
            child->pRightVal = parent->pRightVal;
            child->iLower = parent->index;
            child->iUpper = parent->iUpper;
            child->pLeft = parent;
            child->pRight = parent->pRight;

            samplePrep(child,level+1,steps);
        }
        else
            return;
    }
}

void writeToFile(node *currNode/*, int lIndex, int uIndex*/)
{
    FILE *file = fopen("output.txt", "a+");
    fprintf(file, "%10d   ", currNode->index);
    fprintf(file, "%10d   ", currNode->pLeftVal);
    fprintf(file, "%10d   ", currNode->iLower /*lIndex*/);
    fprintf(file, "%10d   ", currNode->pRightVal);
    fprintf(file, "%10d   ", currNode->iUpper /*uIndex*/);
    fprintf(file, "%10d   ", currNode->val);
    fprintf(file, "%10d   ", currNode->inDeg);
    fprintf(file, "%10d   ", currNode->outDeg);
    fprintf(file, "%10d   \n", currNode->inDeg - currNode->outDeg);

    fclose(file);

}

void vol_calc(node *currNode, int *waste, int *finalVolOutput)
{
    if (currNode->cLeft == NULL && currNode->cRight == NULL)
    {
        // printf("hitting base case\n");
        currNode->outDeg = 0;

        currNode->inDeg =2;
        //  printf("pleft's outDeg : %d\n",currNode->pLeft->outDeg);
        currNode->pLeft->outDeg += 1;
        //  printf("OK!\n");
        currNode->pRight->outDeg += 1;
        *finalVolOutput += currNode->inDeg;
        //  printf("inDeg %d\n",currNode->inDeg);
        //  printf("Before return statement\n");
        return;
    }

    if (currNode->cLeft != NULL)
        vol_calc(currNode->cLeft, waste, finalVolOutput);
    if (currNode->cRight != NULL)
        vol_calc(currNode->cRight, waste, finalVolOutput);

    // The indegree must always be greater equalto (>=) than the outdegree , otherwise multiple the ratio by a whole number
    if (2 >= currNode->outDeg)
    {
        currNode->inDeg = 2;
        currNode->pLeft->outDeg += 1;
        currNode->pRight->outDeg += 1;
        *waste += currNode->inDeg - currNode->outDeg;
    }
    else
    {
        int ratioMultx = 2;
        while (1)
        {
            if (2 * ratioMultx >= currNode->outDeg)
                break;
            else
                ratioMultx++;
        }
        currNode->inDeg = 2 * ratioMultx;
        currNode->pLeft->outDeg += 1 * ratioMultx;
        currNode->pRight->outDeg += 1 * ratioMultx;
        *waste += currNode->inDeg - currNode->outDeg;
    }

    return;
}

/*
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
*/
void display(node *currNode,/* node *constRoot,*/ node **list, int start, int tail/*, int solution*/)
{
    /*
    int lIndex = lowerIndex(currNode, constRoot);
    int uIndex = upperIndex(currNode, constRoot, solution);
    */
   // printf("write to file calling \n");
    writeToFile(currNode/*, lIndex, uIndex*/); // Prining all the info to the file
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
        display(list[start],/* constRoot,*/ list, start, tail/*, solution*/);
}

int main()
{
    int steps;
    printf("Enter the number of steps : ");
    scanf("%d", &steps);

     // Creating the solution and buffer node
    node *solution = createNode();
    node *buffer = createNode();

    solution->index = -2;
    solution->val = pow(2, steps);

    buffer->index = -1;
    buffer->val = 0;

    //int buffer = 0, solution = pow(2, steps);

    node *root = createNode();
    root->index = 0;
    root->val = (buffer->val + solution->val) / 2;
    root->pLeftVal = buffer->val;
    root->pRightVal = solution->val;
    root->iLower = buffer->index;
    root->iUpper = solution->index;
    root->pLeft = buffer;
    root->pRight = solution;

    int level = 1; // As the root node is already created which is at level 0
    samplePrep(root, level, steps);

    int waste = 0, finalVolOutput = 0;
    vol_calc(root, &waste, &finalVolOutput);

    node *list[(int)(pow(2.0, steps))]; // max size of the BFS array is 2^steps
    // list[0] = root;
    //  Initialsing all list array elements as NULL
    for (int i = 0; i < (int)(pow(2.0, steps)); i++)
        list[i] = NULL;

    // If segmentation error shows then comment the following FILE para
    
    FILE *file = fopen("output.txt", "w");
    fprintf(file, "%10s   ", "Index");
    fprintf(file, "%10s   ", "Lower_CF");
    fprintf(file, "%10s   ", "Lower_Index");
    fprintf(file, "%10s   ", "Upper_CF");
    fprintf(file, "%10s   ", "Upper_Index");
    fprintf(file, "%10s   ", "Resultant_CF");
    fprintf(file, "%10s   ", "In-degree");
    fprintf(file, "%10s   ", "Out-degree");
    fprintf(file, "%10s   \n", "Volume");
    fclose(file);
    
    int start = 0, tail = 0;
    // printf("Calling display to write to file \n");
    display(root, /* root, */ list, start, tail /*, solution*/);
    

    file = fopen("output.txt", "a+");
    fprintf(file, "\n%10s %d  ", "Buffer_Vol", buffer->outDeg);
    fprintf(file, "%10s %d  ", "Solution_Vol", solution->outDeg);
    fprintf(file, "%10s %d  ", "Waste_Vol", waste);
    fprintf(file, "%10s %d  ", "Output_Vol", finalVolOutput);
    fprintf(file, "%10s %d  \n", "Dilutions", (int)(pow(2, steps) - 1));
    fclose(file);

    printf("Data successfully stored in 'output.txt' file\n");
    return 0;
}