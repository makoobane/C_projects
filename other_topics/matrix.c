#include <stdio.h>
#include <stdlib.h>

int main(){
    int diagnalsSum=0;
    int side;
    puts("enter what is side length of your square matrix, nrows and ncols ");
    scanf("%d",&side);
    //allocating 2D array
    int **arr=malloc(side*sizeof(int*));
    for(int j=0;j<side;j++){
     arr[j]=malloc(side*sizeof(int));
    }
    //fill the array
    for(int k=0;k<side;k++){
        for(int h=0;h<side;h++){
            arr[k][h]=h+1+k*side;
        }
    }
    //displaying 2D array
    for(int k=0;k<side;k++){
        for(int h=0;h<side;h++){
         printf("%d     ",arr[k][h]);
        }
        printf("\n");
    }
    //sum of diagnal (leading diagnal)
    //what formula i need for finding leading diagnal . one element is [0][0] another one is [side-1][side-1] what formula links???
    // it is easy from 0 to side-1 2D like [0][0],[1][1],.....[5][5]....[side-1][side-1] so normal for loop of 0 to <side
    int leadingSum=0;
    for(int m=0;m<side;m++){
        leadingSum+=arr[m][m];
    }
    printf("leading sum is %d\n",leadingSum);
    // what about secondary diagnal? [0][side-1],[1][side-2]....[side-1][0] 
    int secondaryDiagnalSum=0;
    for(int a=0;a<side;a++){
       secondaryDiagnalSum+=arr[a][side-1-a];
    }
    printf("secondary sum is %d\n",secondaryDiagnalSum);

    //calculating sum of two diagnals was the challenge .... since now it is decomposed we need to remove duplicates. 
    // if side is even number no duplicates for the sum is what i observed in s=2,4,6 but odd side-length makes duplicate
    // and that duplicate is [floor(side/2)][floor(side/2)]
    if(side%2==0){
        //even
        diagnalsSum=leadingSum+secondaryDiagnalSum;
    }else{
        //odd
        int pos=side/2;
        int intercept=arr[pos][pos];
        diagnalsSum=leadingSum+secondaryDiagnalSum-intercept;
    }
    printf("total diagnals sum is :%d\n",diagnalsSum);
   
    //de-alocating 2D array
    for(int i=0;i<side;i++){
        free(arr[i]);
    }
    free(arr);

    return 0;
}