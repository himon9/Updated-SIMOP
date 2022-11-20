#include <stdio.h>
#include <string.h>

int main()
{

    

    

    int fileNo=2;
    char src[50]="./OutputFiles/";
    char filename[20]="output";
    char outputfileNo[10];

    sprintf(outputfileNo, "%d", fileNo);
    strcat(filename,outputfileNo);
    strcat(filename,".txt");
    strcat(src,filename);

    FILE *file =fopen(src,"a+");
    printf("Filename ");
    fprintf(file,"Filename ");
    fclose(file);
}