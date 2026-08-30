#include <stdio.h>
#include <stdlib.h>

int* findPairs(int* arr, int length, int sum);
int main(){
    int n;
    printf("enter length of your upcoming array: ");
    scanf("%d",&n);
    int *arr=calloc(n,sizeof(int));
    if(arr!=NULL){
        for(int i=0;i<n;i++){
            printf("enter element %d of the %d :",i+1,n);
            scanf("%d",arr+i);
            while(getchar()!='\n');
        }
     int sum;
    puts("enter the sum that you are looking from array");
    scanf("%d",&sum);
    int* indexes=findPairs(arr,n,sum);
    if(indexes!=NULL){
        //show two numbers those adds up [sum]
        if(indexes[0]==-1 && indexes[1]==-1){
            puts("no sum was found");
        }else{
            printf("sum %d=arr[%d]+arr[%d]\n",sum,indexes[0],indexes[1]);
        }
        free(indexes);
        indexes=NULL;
    }else{
        perror("it cant allocate that indexes array");
        exit(1);
    }
    free(arr);
    arr=NULL;
    }else{
        perror("it failed to allocate that array memory");
        exit(1);
    } 
    
    return 0;
}

int *findPairs(int *arr, int length, int sum)
{ 
  int* two_indexes=malloc(2*sizeof(int));
  for(int k=0;k<length;k++){
    two_indexes[0]=k;
    for(int h=0;h<length;h++){
        if(h==k) continue;
        if(arr[h]+arr[two_indexes[0]]==sum){
            two_indexes[1]=h;
            return two_indexes;
        }
    }
  }
  two_indexes[0]=-1;
  two_indexes[1]=-1;
  return two_indexes;
}
