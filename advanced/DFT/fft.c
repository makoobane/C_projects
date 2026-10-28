#include <stdlib.h>
#include <stdio.h>
struct Complex
{
    float real;
    float imaginery;
};
void printArrayComplex(struct Complex* complex,size_t size);
void find4Data_FFT(float* data,struct Complex* fft);

int main(){
    float* data=malloc(4*sizeof(float));
    struct Complex* fft=malloc(4*sizeof(struct Complex));
    data[0]=10;
    data[1]=11;
    data[2]=12;
    data[3]=13;
    find4Data_FFT(data,fft);
    
    // cleaning
    free(data);
    free(fft);
    return 0;
}

void printArrayComplex(struct Complex *complex, size_t size)
{
    printf("[");
    for(size_t i=0;i<size; i++){
        struct Complex current=complex[i];
        if(current.imaginery!=0){
            printf("%f %fi,",current.real,current.imaginery);
        }else{
            printf("%f,",current.real);
        }
    }
    printf("]\n");
}
// this function is just for 4 data points
void find4Data_FFT(float *data, struct Complex *fft)
{
    size_t length=4;
    int evenIndices[2]={0,2};
    int oddIndices[2]={1,3};
    for(size_t i=0;i<length/2;i++){
       float evenAdd=(float)(data[evenIndices[0]]+data[evenIndices[1]]);
       float evenSub=(float)(data[evenIndices[0]]- data[evenIndices[1]]);
       float oddAdd=(float)(data[oddIndices[0]]+data[oddIndices[1]]);
       float oddSub=(float)(data[oddIndices[0]]-data[oddIndices[1]]);
       

    }
}
