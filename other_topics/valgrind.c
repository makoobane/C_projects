#include <stdio.h>
#include <stdlib.h>

int main(){
    int* ten=calloc(10,sizeof(int));
    ten[19]=13;
    free(ten);
    ten=NULL;
    return 0;
}