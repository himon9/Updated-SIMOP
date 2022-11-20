#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define totPreds 20    // Total pre-decessors is maximum 10
#define maxChildren 20 // Total pre-decessors is maximum 10
int buffer = 0, solution = 128;

typedef struct TreeNode Node;
struct TreeNode
{
    Node *parent;
    Node *children;
    Node *siblings;
    int value, steps /*,inDeg,outDeg,volume*/;
    int X, Y;
};

Node *createNode(int value)
{
    Node *newNode = (Node *)calloc(1, sizeof(Node));
    newNode->value = value;
    newNode->steps = 0;
    // newNode->inDeg=0;
    // newNode->outDeg=0;
    // newNode->volume=0;

    newNode->parent = NULL;
    newNode->children = NULL;

    newNode->X = buffer;
    newNode->Y = solution;
    return newNode;
}

int *getPreds(Node *node)
{

    int *preds = (int *)calloc(totPreds, sizeof(int));
    int counter = 0;

    preds[counter++] = buffer;
    while (node->parent)
    {
        node = node->parent;
        preds[counter++] = node->value;
    }
    preds[counter] = solution;

    // printf("Printing preds: ");
    // for (int i = 0; i <= counter; i++)
    //  printf("%d ", preds[i]);
    // printf("\n");

    return preds;
}

void children_init(Node *currNode, int *preds, int predSize, int *hashMap)
{
    if (currNode->value % 2 != 1)
    {
        int *children = (int *)calloc(predSize, sizeof(int));
        // printf("Chidren: ");
        for (int i = 0; i < predSize; i++)
        {
            int childVal = (currNode->value + preds[i]) / 2;

            // if (!hashMap[childVal])//Comment this if statement to generate all possible combinations
            {
                hashMap[childVal] = 1;

                // Creating child node
                Node *child = createNode(childVal);
                // printf("%d ", child->value);

                child->parent = currNode;
                child->X = preds[i];
                child->Y = currNode->value;
                // child->inDeg+=1;

                if (currNode->children == NULL)
                {
                    currNode->children = child;
                }
                else
                {
                    Node *traverse = currNode->children;
                    while (traverse->siblings)
                    {
                        traverse = traverse->siblings;
                    }
                    traverse->siblings = child;
                }
            }
        }
    }
}

int counter = 1;
void nodeHistory(Node *currNode, int **props, int propsRow, FILE *file)
{

    /*fprintf(file,"\n\nSerial No :- %d\n",counter++);
    fprintf(file,"Current Node: %d\n", currNode->value);
    fprintf(file,"Node creation history: ");*/
    // fprintf(file,"(%d+%d)/2=%d  ",currNode->X,currNode->Y,currNode->value);
    // fprintf(file,"[%d,%d,%d] ",currNode->X,currNode->Y,currNode->value);
    props[propsRow][0] = currNode->X;
    props[propsRow][1] = currNode->Y;
    props[propsRow][2] = currNode->value;

    // printf("(%d+%d)/2=%d  ",currNode->X,currNode->Y,currNode->value);
    if (currNode->parent)
        nodeHistory(currNode->parent, props, ++propsRow, file);
}

void tree_init(Node *currNode, int *hashMap)
{

    int fileNo = currNode->value;
    char src[50] = "./OutputFiles/";
    char filename[20] = "output";
    char outputfileNo[10];

    sprintf(outputfileNo, "%d", fileNo);
    strcat(filename, outputfileNo);
    strcat(filename, ".txt");
    strcat(src, filename);

    FILE *file = fopen(src, "a+");
    // fprintf(file, "\nSerial_No:-%d,   ", counter++);
    fprintf(file, "\nTarget:'%d',   ", currNode->value);
    int *preds = getPreds(currNode);
    int predSize = 0;
    while (preds[predSize++] != solution)
        ; // since solution always appear at the last index of the pred array
    // predSize--; //Calculates the total number of preds
    // printf("predsize: %d\n", predSize);
    currNode->steps = predSize - 1;
    fprintf(file, "Steps:%d,    ", currNode->steps);
    children_init(currNode, preds, predSize, hashMap);

    int **props = (int **)calloc(totPreds, sizeof(int *)); // 2D matrix for storing props like waste sample buffer
    for (int i = 0; i < totPreds; i++)
        props[i] = (int *)calloc(3, sizeof(int));
    int propsRow = 0;

    nodeHistory(currNode, props, propsRow, file);

    //
    int totParents = 0;
    for (int i = 0; i < totPreds; i++)
    {
        if (props[i][2] == 0)
        {
            totParents = i - 1;
            break;
        }
    }


    int totSample = 0, totBuffer = 0, totWaste = 0;
   /* for (int i = 0; i < totPreds; i++)
    { // Calculating sample buffer solution
        if (props[i][2] == 0)
        {
            // totParents = i - 1;
            break;
        }

        else
        {
            if (props[i][0] == solution || props[i][1] == solution)
                totSample++;
            if (props[i][0] == buffer || props[i][1] == buffer)
                totBuffer++;
        }
    }
    */
    int *propsHash = (int *)calloc(solution + 1, sizeof(int));
    for (int i = 0; i <= totParents; i++)
    {
        propsHash[props[i][0]]++;
        propsHash[props[i][1]]++;
        propsHash[props[i][2]]++;
    }
    totSample=propsHash[solution];
    totBuffer=propsHash[buffer];
    fprintf(file, "Buffer: %d,   Solution: %d    ", totBuffer, totSample);
    fprintf(file, "Method: ");

    

    for (int i = totParents; i >=0; i--)
    {
        if (props[i][2] == 0)
            break;
        fprintf(file, "[%d,%d,%d] ", props[i][0], props[i][1], props[i][2]);
    }
    fclose(file);

    if (currNode->children)
        tree_init(currNode->children, hashMap);

    if (currNode->siblings)
        tree_init(currNode->siblings, hashMap);
}

int main()
{

    double time_spent = 0.0;
    clock_t begin = clock();

    int *resultHash = (int *)calloc(solution, sizeof(int)); // 1-to-15 and they are initialised with 0

    Node *root = createNode((buffer + solution) / 2);

    tree_init(root, resultHash);

    // fclose(file);
    // For merging the files

    /*
    FILE *allOutput = fopen("allOutput.txt", "a+");
    for (int i = 1; i < solution; i++)
    {
        char src[50] = "./OutputFiles/";
        char filename[20] = "output";
        char outputfileNo[10];

        sprintf(outputfileNo, "%d", i);
        strcat(filename, outputfileNo);
        strcat(filename, ".txt");
        strcat(src, filename);
        FILE *outputFile = fopen(src, "r");

        char c = fgetc(outputFile);
        while (c != EOF)
        {
            fputc(c, allOutput);
            c = fgetc(outputFile);
        }
        fprintf(allOutput, "\n\n");
        fclose(outputFile);
        remove(src);
    }

    fclose(allOutput);*/
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n\nElapsed Time : %f seconds", time_spent);

    return 0;
}
