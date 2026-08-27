#include <stdio.h>
#include <stdlib.h>
int* removeDuplicates(int* arr,int length);
int contains(int* arr, int length, int element);
int setlength=0;
int main(){
    int length;
    puts("Enter how many numbers you need to enter");
    scanf("%d",&length);
    int* array=malloc(length*sizeof(int));
    if(array!=NULL){
          int k=0;
    do{
        printf("enter element %d of total %d :\n",k+1,length);
        scanf("%d",array+k);
        k++;
        // while(getchar()!="\n");
    }while(k<length);
    int* set=  removeDuplicates(array,length);
     for(int n=0;n<setlength;n++){
        printf("%d",set[n]);
     }
     printf("\n");
    
    free(array);
    free(set);
    array=NULL;
    set=NULL;
    }else{
        perror("bro it failed to allocate the  memory you wanted");
        exit(1);
    }

    return 0;
}
int contains(int* arr, int length,int element){
 for(int i=0;i<length;i++){
    if(arr[i]==element){
        return 1;
    }
 }
 return 0;
}

int* removeDuplicates(int* arr, int length){//while keeping the order
    int duplicats_count=0;
    int k=1;
    do{
        int cont=contains(arr,k,arr[k]);
        if(cont){
          duplicats_count+=1;
         }
        k+=1;
     }while(k<length);
    printf("duplicats_count:%d\n",duplicats_count);
    int* set=malloc((length-duplicats_count)*sizeof(int));
    if(set!=NULL){
            int whereWeHitDuplicate=-1;
    for(int m=0;m<length;m++){
        int contained=contains(arr,m,arr[m]);
        if(contained){
            whereWeHitDuplicate=whereWeHitDuplicate==-1? m:whereWeHitDuplicate;
        }else{
            if(whereWeHitDuplicate==-1){
             set[m]=arr[m];
            }else{
              set[whereWeHitDuplicate]=arr[m];
              whereWeHitDuplicate+=1;
            }
        }
    }
 setlength=length-duplicats_count;
 return set;
    }
    return NULL;
}