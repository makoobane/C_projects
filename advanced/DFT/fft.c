#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "complex.h"
void FFT_printCoefficients(struct ComplexNumber* coefficients,size_t size);
struct ComplexNumber* FFT_2(int16_t* data);
void FFT_spliting(struct ComplexNumber* coefficients, int size);
struct ComplexNumber* FFT_4(int16_t* data,size_t size);
int main(){
    size_t size=16;
    int16_t* data=calloc(size,sizeof(int16_t));
    for(size_t i=0;i<size;i++){
        data[i]=(int16_t)i;
    }
    struct ComplexNumber* coefficients=calloc(size,sizeof(struct ComplexNumber));
    if(coefficients==NULL){
        puts("it failed to allocate four data points coefficients");
    }
    // put data in coefficients as complex
    for(size_t s=0;s<size;s++){
        coefficients[s].real=(float)data[s];
        coefficients[s].imaginery=0.0;
    }
    FFT_spliting(coefficients,size);
    FFT_printCoefficients(coefficients,size);
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
void FFT_spliting(struct ComplexNumber *coefficients, int size)
{
    struct ComplexNumber* spareCoefficients=calloc(size,sizeof(struct  ComplexNumber));
    if(spareCoefficients==NULL){
        puts("spare allocation failed in splitting");
        return ;
    }
    int bits=(int)log2f(size);
    printf("bit size is %d\n",bits);
    int reversed_index=0;
    for(int index=0;index<size;index++){
        int currentIndex=index;
        for(int b=0;b<bits;b++){
            //shift to left to make space for new binary
            reversed_index<<=1;
            //cut last binary in index and put it on reversal
            reversed_index |= (currentIndex & 1);
            //remove that last binary from index
            currentIndex >>= 1;
        }
        //now we have reversed index
        spareCoefficients[reversed_index].real=coefficients[index].real;
        spareCoefficients[reversed_index].imaginery=coefficients[index].imaginery;
        //reset reversed_index
        reversed_index=0;
    }    
    //unload those changes to real coeficients
    for(int j=0;j<size;j++){
        coefficients[j].real=spareCoefficients[j].real;
        coefficients[j].imaginery=spareCoefficients[j].imaginery;
    }
    free(spareCoefficients);
}
/// @brief  this function is trying to do FFT of four data points at this time
struct ComplexNumber *FFT_4(int16_t *data,size_t size)
{
    struct ComplexNumber* coefficients=calloc(size,sizeof(struct ComplexNumber));
    if(coefficients==NULL){
        puts("it failed to allocate four data points coefficients");
        return NULL;
    }
    // put data in coefficients as complex
    for(size_t s=0;s<size;s++){
        coefficients[s].real=(float)data[s];
        coefficients[s].imaginery=0.0;
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
            coefficients[k].real=addition->real;
            coefficients[k].imaginery=addition->imaginery;
            
            free(twiddle);
            free(secondComplexWithTwiddle);
            free(addition);
            free(subtraction);

        }
    }
    return coefficients;
}
