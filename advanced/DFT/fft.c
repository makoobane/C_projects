#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "complex.h"
void FFT_printCoefficients(struct ComplexNumber* coefficients,size_t size);
struct ComplexNumber* FFT_forTwoDataPoints(int16_t* data);
struct ComplexNumber* FFT_forFourDataPoints(int16_t* data);
int main(){
    // size_t two=2;
    size_t four=4;
    int16_t* data=calloc(four,sizeof(int16_t));
    data[0]=10;
    data[1]=13;
    data[2]=9;
    data[3]=3;
    // struct ComplexNumber* coefficients= FFT_forTwoDataPoints(data);
    // FFT_printCoefficients(coefficients,two);
    struct ComplexNumber* coefficients=FFT_forFourDataPoints(data);
    FFT_printCoefficients(coefficients,four);
    // freeing
    free(data);
    free(coefficients);
    return 0;
}

void FFT_printCoefficients(struct ComplexNumber *coefficients,size_t size)
{
    for(size_t s=0;s<size;s++){
        printComplex(coefficients[s]);
    }
}
struct ComplexNumber* FFT_forTwoDataPoints(int16_t* data)
{
    struct ComplexNumber* coefficients=calloc(2,sizeof(struct ComplexNumber));
    if(coefficients==NULL){
        puts("coefficients failed in two datapoints calculation");
        return NULL;
    }
    float coef1=(float)(data[0]+data[1]);
    float coef2=(float)(data[0]-data[1]);
    coefficients[0].real=coef1;
    coefficients[0].imaginery=0.0;
    coefficients[1].real=coef2;
    coefficients[1].imaginery=0.0;
    return coefficients;

}

struct ComplexNumber *FFT_forFourDataPoints(int16_t *data)
{
    struct ComplexNumber* coefficients=calloc(4,sizeof(struct ComplexNumber));
    if(coefficients==NULL){
        puts("it failed to allocate four data points coefficients");
        return NULL;
    }
    //do twiddle of twos
    float coef1=(float)(data[0]+data[2]);
    float coef2=(float)(data[0]-data[2]);
    float coef3=(float)(data[1]+data[3]);
    float coef4=(float)(data[1]-data[3]);
    coefficients[0].real=coef1;
    coefficients[0].imaginery=0.0;
    coefficients[1].real=coef2;
    coefficients[1].imaginery=0.0;
    coefficients[2].real=coef3;
    coefficients[2].imaginery=0.0;
    coefficients[3].real=coef4;
    coefficients[3].imaginery=0.0;
    //do twiddle of four
    struct ComplexNumber* twiddle0=createComplexFromKN(0,4);
    struct ComplexNumber* twiddle1=createComplexFromKN(1,4);
    //calculate twidlle multiples
    struct ComplexNumber* coef3WithTwiddle0=multiply(&coefficients[2],twiddle0);
    struct ComplexNumber* coef4WithTwiddle1=multiply(&coefficients[3],twiddle1);
    free(twiddle0);
    free(twiddle1);
    coefficients[0].real+=coef3WithTwiddle0->real;
    coefficients[0].imaginery+=coef3WithTwiddle0->imaginery;
    coefficients[1].real-=coef3WithTwiddle0->real;
    coefficients[1].imaginery-=coef3WithTwiddle0->imaginery;
    coefficients[2].real+=coef4WithTwiddle1->real;
    coefficients[2].imaginery+=coef4WithTwiddle1->imaginery;
    coefficients[3].real-=coef4WithTwiddle1->real;
    coefficients[3].imaginery-=coef4WithTwiddle1->imaginery;

    free(coef3WithTwiddle0);
    free(coef4WithTwiddle1);    
    return coefficients;
}
