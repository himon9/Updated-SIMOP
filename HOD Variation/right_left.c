#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Now creating concentration_factor
typedef struct concentration_factor node;

struct concentration_factor
{
    int index;
    int val;
    int outDeg;
    node *pLeft, *pRight;
};

node *createNode()
{
    node *store = (node *)malloc(sizeof(node));
    store->index = 0;
    store->val = 0;
    store->outDeg = 0;
    store->pLeft = NULL;
    store->pRight = NULL;

    return store;
}

int findCF(node **uniqueNodes, int nodeCounter, int cf)
{
    for (int i = 0; i < nodeCounter; i++)
    {
        if (uniqueNodes[i]->val == cf)
            return 1;
    }
    return 0;
}

int main()
{
    int steps;
    printf("Enter the number of steps : ");
    scanf("%d", &steps);

    node *solution = createNode();
    node *buffer = createNode();

    solution->index = 1;
    solution->val = pow(2, steps);

    buffer->index = 0;
    buffer->val = 0;

    node **uniqueNodes = (node **)malloc(sizeof(node *) * ((int)pow(2, steps) + 1));
    int nodeCounter = 0;

    uniqueNodes[nodeCounter++] = solution;
    uniqueNodes[nodeCounter++] = buffer;

    int totalNodes = pow(2, steps)+1;
    printf("Total Nodes : %d\n",totalNodes);
    int flag = 0;
    while (1)
    {
        for (int i = 0; i < nodeCounter; i++)
        {
            for (int j = i + 1; j < nodeCounter; j++)
            {
                double cf = (uniqueNodes[i]->val + uniqueNodes[j]->val) / 2.0;
                // Checking if cf is an interger
                if (ceil(cf) == cf)
                {                    
                    printf("Current CF : %f\n",cf);
                    int present = findCF(uniqueNodes, nodeCounter, (int)cf);
                    if (present == 0)
                    {
                        node *newNode = createNode();

                        newNode->pLeft = uniqueNodes[i];
                        newNode->pRight = uniqueNodes[j];

                        newNode->index = nodeCounter;
                        newNode->val = (uniqueNodes[i]->val + uniqueNodes[j]->val) / 2;

                        newNode->pLeft->outDeg++;
                        newNode->pRight->outDeg++;

                        uniqueNodes[nodeCounter++] = newNode;
                    }
                    printf("nodeCounter : %d\n",nodeCounter);
                }
                if (nodeCounter == totalNodes)
                {
                    flag = 1;
                    break;
                }
            }
            if (flag)
                break;
        }

        if (flag)
            break;
    }

    /*for(int i=2;i<=totalNodes;i++)
    {
        printf("Index : %d\n",uniqueNodes[i]->index);
        printf("Lower CF : %d\n",uniqueNodes[i]->pLeft->val);
        printf("Upper CF : %d\n",uniqueNodes[i]->pRight->val);
        printf("Resultant CF : %d\n",uniqueNodes[i]->val);
        printf("outDeg : %d\n\n",uniqueNodes[i]->outDeg);
    }*/
    // Printing the data to file
     FILE *file = fopen("right_left.txt", "a+");
    fprintf(file, "%10s   ", "Index");
    fprintf(file, "%10s   ", "Lower_CF");
    fprintf(file, "%10s   ", "Upper_CF");
    fprintf(file, "%10s   ", "Resultant_CF");
    fprintf(file, "%10s   \n", "Out-degree");
  //  fclose(file);

    //file=fopen("right_left.txt", "a+");
    for (int i = 2; i <totalNodes; i++)
    {
       // printf("Inside the for loop\n");
        fprintf(file, "%10d   ", uniqueNodes[i]->index);
        fprintf(file, "%10d   ", uniqueNodes[i]->pLeft->val);        
        fprintf(file, "%10d   ", uniqueNodes[i]->pRight->val);        
        fprintf(file, "%10d   ", uniqueNodes[i]->val);        
        fprintf(file, "%10d   \n", uniqueNodes[i]->outDeg);
    }
    fclose(file);

    printf("Data written to file successfully\n");
    return 0;
}