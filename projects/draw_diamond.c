#include <stdio.h>
//   *
//  ***
// *****
//  ***
//   *
int main(){
    int max=5;
    //first triangle
    for(int i=1;i<=max;i+=2){
        int space_side=(max-i)/2;
        // printf("at %d space side is %d\n",i,space_side);
       if(space_side!=0) printf("%*c",space_side,' ');
        for(int j=0;j<i;j++){
            printf("%c",'*');
        }
       if(space_side!=0) printf("%*c",space_side,' ');
       printf("\n");
    }
    //bottom part or lower small triangle
    for(int i=3;i>=1;i-=2){
        int space_side=(max-i)/2;
        // printf("at %d space side is %d\n",i,space_side);
       if(space_side!=0) printf("%*c",space_side,' ');
        for(int j=0;j<i;j++){
            printf("%c",'*');
        }
       if(space_side!=0) printf("%*c",space_side,' ');
       printf("\n");
    }
    

    return 0;
}