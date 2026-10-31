#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "complex.h"

void FFT_twoDataPoints(size_t twin,int16_t* data,struct ComplexNumber* coefficients);
void printCoefficients(struct ComplexNumber* coefficients,size_t size);
int main(){
    size_t twin=2;
    int16_t* data=calloc(twin,sizeof(int16_t));
    struct ComplexNumber* coefficients=calloc(twin,sizeof(struct ComplexNumber));
    data[0]=10;
    data[1]=20;
    FFT_twoDataPoints(twin,data,coefficients);
    printCoefficients(coefficients,twin);
    // free
    free(coefficients);
    free(data);
    return 0;
}

void FFT_twoDataPoints(size_t twin, int16_t *data,struct  ComplexNumber *coefficients)
{
    if(twin!=2){
        puts("your data length is not two");
        return;
    }
    //no spliting 
    /// just butterfly them 
    struct ComplexNumber* complex=createComplexFromKN(0 , twin);
    scaleComplex((double)data[1],complex);
    addNumberToComplex(complex,data[0]);
    coefficients[0]=complex[0];
    free(complex);
    struct ComplexNumber* complex2=createComplexFromKN(0 , twin);
    scaleComplex((double)data[1],complex);
    subtractComplexFromNumber(complex,data[0]);
    coefficients[1]=complex[0];
    free(complex);
}

void printCoefficients(struct ComplexNumber *coefficients, size_t size)
{
    for(size_t s=0;s<size;s++){
        printComplex(coefficients[s]);
    }
}
