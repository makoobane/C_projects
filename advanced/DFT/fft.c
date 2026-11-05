#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "complex.h"
void FFT_printCoefficients(struct ComplexNumber* coefficients,size_t size);
struct ComplexNumber* FFT_2(int16_t* data);
struct ComplexNumber* FFT_4(int16_t* data,size_t size);
int main(){
    // size_t two=2;
    size_t four=4;
    int16_t* data=calloc(four,sizeof(int16_t));
    data[0]=10;
    data[1]=11;
    data[2]=12;
    data[3]=13;
    // struct ComplexNumber* coefficients= FFT_2(data);
    // FFT_printCoefficients(coefficients,two);
    struct ComplexNumber* coefficients=FFT_4(data,four);
    FFT_printCoefficients(coefficients,four);
    // freeing
    free(data);
    free(coefficients);
    return 0;
}

void FFT_printCoefficients(struct ComplexNumber *coefficients,size_t size)
{
    printf("--------------------- complex numbers of %zu data points -------\n",size);
    for(size_t s=0;s<size;s++){
        printf("coefficients[%zu]: ",s);
        printComplex(coefficients[s]);
    }
    puts("=========================end ===");
}
struct ComplexNumber* FFT_2(int16_t* data)
{
    struct ComplexNumber* coefficients=calloc(2,sizeof(struct ComplexNumber));
    if(coefficients==NULL){
        puts("coefficients failed in two datapoints calculation");
        return NULL;
    }
    float coef1=(float)(data[0]+data[1]);
    float coef2=(float)(data[0]-data[1]);
    coefficients[0].real=coef1;
    coefficients[1].real=coef2;
    //calloc fills zeros on imaginary
    return coefficients;

}
/// @brief  this function is trying to do FFT of four data points at this time
struct ComplexNumber *FFT_4(int16_t *data,size_t size)
{
    struct ComplexNumber* coefficients=calloc(4,sizeof(struct ComplexNumber));
    if(coefficients==NULL){
        puts("it failed to allocate four data points coefficients");
        return NULL;
    }
    // put data in coefficients as complex
    for(size_t s=0;s<4;s++){
        coefficients[s].real=(float)data[s];
        //calloc will ffill zeros in imaginary
    }
    float howmanyTwiddles=log2f(size);
    size_t nyquest=size/2;
    printf("how many twiddles i need:%f\n",howmanyTwiddles);
    for(int t=0;t<(int)howmanyTwiddles;t++){
        // first twiddle is 2 data points connection, second twiddle is for four data points connection...
        uint32_t N=(uint32_t)pow(2,t+1);
        for(size_t k=0;k<nyquest;k++){
            //this spliting of four data points 
            struct ComplexNumber firstComplex=coefficients[k];
            struct ComplexNumber secondComplex=coefficients[k+nyquest];
            uint32_t K=t*k;
            struct ComplexNumber* twiddle=createComplexFromKN(K,N);
            struct ComplexNumber* secondComplexWithTwiddle=multiply(&secondComplex,twiddle);
            struct ComplexNumber* addition=addTwoComplexes(&firstComplex,secondComplexWithTwiddle);
            struct ComplexNumber* subtraction=subtractComplexes(&firstComplex,secondComplexWithTwiddle);
            coefficients[k].real=addition.real;
            coefficients[k].imaginery=addition->imaginery;
            
            free(twiddle);
            free(secondComplexWithTwiddle);
            free(addition);
            free(subtraction);

        }
    }
    return coefficients;
}
