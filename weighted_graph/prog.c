#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct fractions
{
    int numerator;
    int denominator;
};
typedef struct fractions frac;

frac *createFraction(int num, int deno)
{
    frac *store = (frac *)(malloc(sizeof(frac)));
    store->numerator = num;
    store->denominator = deno;
    return store;
}

int displayFractions(frac *store)
{

    if (store != NULL)
    {
        FILE *file = fopen("output.txt", "a+");
        fprintf(file, "{%d,%d}  ", (store)->numerator, (store)->denominator);
       //  printf("{%d,%d} ", (store)->numerator, (store)->denominator);
         //printf(" in func ");
        fclose(file);
        return 1;
    }
    return 0;
}

int comparisons = 0;
void swap(frac ***a, frac ***b)
{
    frac **temp = *a;
    *a = *b;
    *b = temp;
}

int fracCombiRowCounter = 0;
void permute(frac ***arr, int n, int pos, frac ***fracCombinations)
{
    // Creating temporary array
    frac **temp[n];
    for (int i = 0; i < n; i++)
        temp[i] = arr[i];

    if (pos == n - 1)
    {
        for (int j = 0; j < n; j++)
        {
            // displayFractions(*(temp+j));
            fracCombinations[fracCombiRowCounter][j] = **(temp + j);
        }
        fracCombiRowCounter++;
        // printf("\n");
        return;
    }

    for (int i = pos; i < n; i++)
    {
        swap((temp + i), (temp + pos));
        comparisons++;
        permute(temp, n, pos + 1, fracCombinations);
    }
}

// Now creating concentration_factor
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

void samplePrep(node *parent, frac **temp, int level, int steps)
{
    // BASE CASE
    if (level == steps)
    {

        /*printf("Base case hit \n");
        printf("level base hit value : %d\n",level);*/
        return;
    }

    if (parent->cLeft == NULL)
    {
        frac *levelRatio = temp[level];
        int leftChildVal = ((parent->val) * (levelRatio->denominator) + (parent->pLeftVal) * levelRatio->numerator) / (levelRatio->denominator + levelRatio->numerator);
        //(parent->val + parent->pLeft) / 2;
        // printf("CurrVal : %d\n", parent->val);
        // printf("leftChildVal : %d\n", leftChildVal);
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

            samplePrep(child, temp, level + 1, steps);
        }
    }
    if (parent->cRight == NULL)
    {
        // printf("entering the right branch \n");
        // printf("Right level value : %d\n",level);
        frac *levelRatio = temp[level];
        int rightChildVal = ((parent->val) * (levelRatio->numerator) + (parent->pRightVal) * levelRatio->denominator) / (levelRatio->denominator + levelRatio->numerator);

        // printf("CurrVal : %d\n", parent->val);
        // printf("rightChildVal : %d\n", rightChildVal);
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

            samplePrep(child, temp, level + 1, steps);
        }
    }
}

int fileCounter = 1;
void writeToFile(node *currNode /*, int lIndex, int uIndex*/)
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
    // printf("file closed \n");

    // For python txt data
    char buffer[50];

    snprintf(buffer, sizeof(buffer), "./DataSet/data%d.txt", fileCounter);
    file = fopen(buffer, "a+");
    fprintf(file, "%d ", currNode->pLeftVal);
    fprintf(file, "%d\n", currNode->val);
    fprintf(file, "%d ", currNode->pRightVal);
    fprintf(file, "%d\n", currNode->val);

    fclose(file);
}

void vol_calc(node *currNode, frac **stepRatio, int stepCounter, int *waste, int *finalVolOutput)
{
    if (currNode->cLeft == NULL && currNode->cRight == NULL)
    {
        // printf("hitting base case\n");
        currNode->outDeg = 0;

        currNode->inDeg = stepRatio[stepCounter]->denominator + stepRatio[stepCounter]->numerator;
        //  printf("pleft's outDeg : %d\n",currNode->pLeft->outDeg);
        currNode->pLeft->outDeg += stepRatio[stepCounter]->numerator;
        //  printf("OK!\n");
        currNode->pRight->outDeg += stepRatio[stepCounter]->denominator;
        *finalVolOutput += currNode->inDeg;
        //  printf("inDeg %d\n",currNode->inDeg);
        //  printf("Before return statement\n");
        return;
    }

    if (currNode->cLeft != NULL)

        vol_calc(currNode->cLeft, stepRatio, stepCounter + 1, waste, finalVolOutput);
    if (currNode->cRight != NULL)
        vol_calc(currNode->cRight, stepRatio, stepCounter + 1, waste, finalVolOutput);

    // The indegree must always be greater equalto (>=) than the outdegree , otherwise multiple the ratio by a whole number
    if ((stepRatio[stepCounter]->denominator + stepRatio[stepCounter]->numerator) >= currNode->outDeg)
    {
        currNode->inDeg = stepRatio[stepCounter]->denominator + stepRatio[stepCounter]->numerator;
        currNode->pLeft->outDeg += stepRatio[stepCounter]->numerator;
        currNode->pRight->outDeg += stepRatio[stepCounter]->denominator;
        *waste += currNode->inDeg - currNode->outDeg;
    }
    else
    {
        int ratioMultx = 2;
        while (1)
        {
            if ((stepRatio[stepCounter]->denominator + stepRatio[stepCounter]->numerator) * ratioMultx >= currNode->outDeg)
                break;
            else
                ratioMultx++;
        }
        currNode->inDeg = (stepRatio[stepCounter]->denominator + stepRatio[stepCounter]->numerator) * ratioMultx;
        currNode->pLeft->outDeg += stepRatio[stepCounter]->numerator * ratioMultx;
        currNode->pRight->outDeg += stepRatio[stepCounter]->denominator * ratioMultx;
        *waste += currNode->inDeg - currNode->outDeg;
    }

    return;
}
/*int lowerIndex(node *currNode, node *root)
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
void display(node *currNode, /*node *constRoot,*/ node **list, int start, int tail /*, int solution*/)
{
    /* int lIndex = lowerIndex(currNode, constRoot);
     int uIndex = upperIndex(currNode, constRoot, solution);*/

    writeToFile(currNode /*, lIndex, uIndex*/); // Prining all the info to the file
    start++;
    if (list[tail + 1] == NULL)
        list[++tail] = currNode->cLeft;
    if (list[tail + 1] == NULL)
        list[++tail] = currNode->cRight;

    // printf("Start : %d , Tail : %d\n", start, tail);
    if (start == tail)
    {
        // printf("Hitting base case of display \n");
        return;
    }

    else
    {
        // printf("hitting else part\n");
        display(list[start], /*constRoot,*/ list, start, tail /*, solution*/);
    }
}

int main()
{

    int steps;
    printf("Enter the number of steps :");
    scanf("%d", &steps);

    // Matrix of level ratios
    // Example 1: (if steps given is 4)
    //  BalancedSteps   WeightedStep  InverseWeightedStep
    //  1               3             0
    //  1               2             1
    //  1               1             2
    //  1               0             3
    //  2               2             0 and so on...

    // for each unique step, there will be different permutations
    /* for example if the step is 1BS, 2WS, 1IWS , then we need to define type1 address array as
       array={1BS,2WS,1IWS}
       and then call permutation and store all possible permutations in a 2d array
    */

    int rows = steps * (steps + 1) / 2 - 1;
    int uniqueSteps[rows][3];
    int counter = steps;

    int rowsArr = 0;
    for (int k = 1;; k++)
    {
        if (k == steps)
            break;
        // uniqueSteps[i][0]=i+1;
        for (int m = 0; m < counter; m++)
        {
            uniqueSteps[rowsArr][0] = k;
            uniqueSteps[rowsArr][1] = counter - m - 1;
            uniqueSteps[rowsArr][2] = steps - (uniqueSteps[rowsArr][0] + uniqueSteps[rowsArr][1]);
            rowsArr++;
        }
        counter--;
    }

    /*
    printf("The possible level ratios : \n");
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<3;j++)
          printf("%d ",uniqueSteps[i][j]);
        printf("\n");
    }
     */

    int v1, v2;

    printf("Enter the volume of c1 sample: ");
    scanf("%d", &v1);
    printf("Enter the volume of c2 sample: ");
    scanf("%d", &v2);

    // for 3 steps, we will have originally three distinct ratios (1 balanced {1:1}& 2 weighted steps {given ratio and its inverse})
    frac *balancedRatio = createFraction(1, 1);
    frac *wtRatio = createFraction(v1, v2);
    frac *wtInvRatio = createFraction(v2, v1);

    // storing the address the addresses of the fraction in an array to implement permutations

    // type 1: {1:1},{1:2},{1:2}
    // type 2: {1:1},{1:2},{2:1}
    // type 3: {1:1},{2:1},{2:1}

    // Total possible level ratio is stored in variable rows
    int uStepsColCounter = 0;
    frac **storeRatioAddType[rows][steps];
    /*storeRatioAddType[0][0]=&balancedRatio;
    storeRatioAddType[0][1]=&wtRatio;
    storeRatioAddType[0][2]=&wtRatio;
    storeRatioAddType[0][3]=&wtRatio;
    storeRatioAddType[0][4]=&wtRatio;
    storeRatioAddType[1][0]=&balancedRatio;
    storeRatioAddType[1][1]=&wtRatio;
    storeRatioAddType[1][2]=&wtRatio;
    storeRatioAddType[1][3]=&wtRatio;
    storeRatioAddType[1][4]=&wtInvRatio;*/

    for (int i = 0; i < rows; i++)
    {
        int colCounter = 0;
        while (uStepsColCounter < 3)
        {
            int nRatioAddrss = uniqueSteps[i][uStepsColCounter]; // cjhnfge++
            for (int j = 0; j < nRatioAddrss; j++)
            {
                switch (uStepsColCounter)
                {
                case 0:
                { // printf("%p ",&balancedRatio);
                    storeRatioAddType[i][colCounter++] = &balancedRatio;
                    break;
                }
                case 1:
                { // printf("%p ",&wtRatio);
                    storeRatioAddType[i][colCounter++] = &wtRatio;
                    break;
                }
                case 2:
                { // printf("%p ",&wtInvRatio);
                    storeRatioAddType[i][colCounter++] = &wtInvRatio;
                    break;
                }
                }
            }
            uStepsColCounter++;
        }
        // printf("\n");
        colCounter = 0;
        uStepsColCounter = 0;
    }

    

    // Creating the storing array

    // Using in-built factorial function
    /*
    Although there is no C function defined specifically for computing factorials, C math library lets you compute gamma function. Since Г(n) = (n-1)! for positive integers, using tgamma of i+1 yields i!.
    */
    int combinations = tgamma(steps + 1) * rows;
    // printf("total rows in fracCombinations : %d\n", combinations);
    int fracCombiRows = combinations, fracCombiCols = steps;
    frac ***fracCombinations = (frac ***)malloc(sizeof(frac **) * fracCombiRows); // taking rows as 18
    for (int i = 0; i < fracCombiRows; i++)
        fracCombinations[i] = (frac **)malloc(sizeof(frac *) * fracCombiCols);

    for (int i = 0; i < rows; i++)
        permute(storeRatioAddType[i], steps, 0, fracCombinations);

    // Now the storeRatioAddType has many duplicate rows and so we need to remove them
    // Now avoiding duplicate rows
    int duplicateRows = 0;
    for (int i = 0; i < fracCombiRows; i++)
    {
        // Now storing the array contents of the present row
        //  Creating temporary array
        frac *temp[steps];
        // printf("printing content in temp array\n");
        for (int k = 0; k < steps; k++)
            temp[k] = fracCombinations[i][k];

        // Now searching for the duplicate values in the rows of fracCombinations

        for (int j = 0; j < fracCombiRows; j++)
        {
            if (j != i && fracCombinations[i][0] != NULL)
            {
                int flag = 1;
                for (int k = 0; k < steps; k++)
                {
                    if (fracCombinations[j][k] != temp[k])
                    {
                        flag = 0;
                        break;
                    }
                }
                // Now making the duplicate row to NULL
                if (flag)
                {
                    duplicateRows++;
                    for (int k = 0; k < steps; k++)
                        fracCombinations[j][k] = NULL;
                }
            }
        }
    }

    // Displaying the stored address in storeRatioAddType array
   /* for(int i=0;i<rows;i++)
    {
        if(storeRatioAddType[i][0]!=NULL)
        {
            for(int j=0;j<steps;j++)
        {
            displayFractions(*storeRatioAddType[i][j]);
            printf (" end ");
        }
        }
        
            
         // printf("%p ",storeRatioAddType[i][j]);
        
        printf("\n");
    }*/
    /*frac **storeRatioAddType1[3] = {&balancedRatio, &wtRatio, &wtRatio};
    frac **storeRatioAddType2[3] = {&balancedRatio, &wtRatio, &wtInvRatio};
    frac **storeRatioAddType3[3] = {&balancedRatio, &wtInvRatio, &wtInvRatio};

    // Dynamically allocating the storin array
    int rows = 18, cols = 3;
    frac ***fracCombinations = (frac ***)malloc(sizeof(frac **) * 18); // taking rows as 18
    for (int i = 0; i < rows; i++)
        fracCombinations[i] = (frac **)malloc(sizeof(frac *) * 3);

    // printf("For type1 : \n");
    permute(storeRatioAddType1, 3, 0, fracCombinations);
    // printf("\nFor type2 : \n");
    permute(storeRatioAddType2, 3, 0, fracCombinations);
    // printf("\nFor type1 : \n");
    permute(storeRatioAddType3, 3, 0, fracCombinations);*/

    /*printf("\nTotal ratio combinations stored in the 2d array:\n");
    int displayCounter = 0, result;
    for (int i = 0; i < fracCombiRows; i++)
    {
        for (int j = 0; j < fracCombiCols; j++)
        {
            result = displayFractions(fracCombinations[i][j]);
        }
        if (result)
        {
            displayCounter++;
            printf("\n");
        }
    }
    printf("total rows : %d\n", fracCombiRowCounter);
    printf("total non null display values :%d\n", displayCounter);
    printf("total duplicate rowss :%d\n", duplicateRows);*/
    // printf("total comparisons : %d", comparisons);

    
    
    
    //fprintf(file, "sep=;\n");
    
    //fprintf(file, "%10s", "SeqNo");
    //fprintf(file, "%10s", "Specs");
   // fprintf(file, "%10s\n", "Volume");
    
    


    // Now creating the solution tree
    for (int i = 0; i < fracCombiRows; i++)
    {
        if (fracCombinations[i][0] != NULL)
        {
            frac *temp[steps];
            for (int k = 0; k < steps; k++)
                temp[k] = fracCombinations[i][k];

            // Displaying the nodes stored in temp
            /* for (int i = 0; i < steps; i++)
                 displayFractions(temp[i]);
             printf("\n");*/
            // Now calculating the solution value
            int nBS = 0, nWS = 0; // nBS=number os balanced steps, nWS=number of weighted steps
            int sBR = 0, sWR = 0; // sBR= sum of balanced ration nume and deno, sWR= sum of
            for (int k = 0; k < steps; k++)
            {

                frac *store = temp[k];
                // printf("Current ratio : %d/%d\n", store->numerator, store->denominator);
                if (store->denominator == store->numerator)
                {
                    sBR = store->denominator + store->numerator;
                    nBS++;
                }
                else
                {
                    sWR = store->denominator + store->numerator;
                    nWS++;
                }
            }

            // printf("nBS : %d , nWS : %d\n", nBS, nWS);
            // printf("sBR : %d , sWR : %d\n", sBR, sWR);

            // Creating the solution and buffer node
            node *solution = createNode();
            node *buffer = createNode();

            solution->index = -2;
            solution->val = pow(sBR, nBS) * pow(sWR, nWS);

            buffer->index = -1;
            buffer->val = 0;

            // int solution = pow(sBR, nBS) * pow(sWR, nWS);
            // int buffer = 0;

            node *root = createNode();
            // root->index = 0;
            root->val = (buffer->val * temp[0]->numerator + solution->val * temp[0]->denominator) / (temp[0]->numerator + temp[0]->denominator);
            root->pLeftVal = buffer->val;
            root->pRightVal = solution->val;
            root->iLower = buffer->index;
            root->iUpper = solution->index;
            root->pLeft = buffer;
            root->pRight = solution;
            // root->cLeft = NULL;
            // root->cRight = NULL;

            //   printf("Root value : %d\n", root->val);
            //  Generating the rest of the sample tree
            int level = 1; // As the root node is already created which is at level 0
            samplePrep(root, temp, level, steps);

            int waste = 0, finalVolOutput = 0;
            vol_calc(root, temp, 0, &waste, &finalVolOutput);

            node *list[(int)(pow(2.0, steps))]; // max size of the BFS array is 2^steps
            // list[0] = root;
            //  Initialsing all list array elements as NULL
            for (int i = 0; i < (int)(pow(2.0, steps)); i++)
                list[i] = NULL;

            FILE *file = fopen("output.txt", "a+");
            fprintf(file, "\n\n\n%75s  %d\n", "Sequence No: ", fileCounter);
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
            fprintf(file, "%10s  ", "Ratio Sequence : ");
            fclose(file);

            for (int i = 0; i < steps; i++)
                displayFractions(temp[i]);
            

            //Now printing data to the csv file
            file=fopen("barGraphData.txt","a+");
            fprintf(file, "%10d  ",fileCounter);
           
            fprintf(file, "%10d  ",finalVolOutput);
           
            
            fprintf(file, "%10d  ",solution->outDeg);
            
            
            fprintf(file, "%10d  ",buffer->outDeg);
            
            
            fprintf(file, "%10d  \n",waste);
            fclose(file);

            fileCounter++;
            // printf("Buffer= %d  , Solution=%d,  Waste=%d,  Output=%d\n",buffer->outDeg,solution->outDeg,waste,finalVolOutput);
        }
    }
    printf("Data written successfully on file 'output.txt' \n");
    printf("File counter: %d\n", fileCounter - 1);
    return 0;
}
