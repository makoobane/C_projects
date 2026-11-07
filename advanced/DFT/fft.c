#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "complex.h"
void FFT_printCoefficients(struct ComplexNumber* coefficients,size_t size);
struct ComplexNumber* FFT_2(int16_t* data);
void FFT_spliting(struct ComplexNumber* coefficients, int size);
struct ComplexNumber* FFT_(int16_t* data,int size);
int main(){
    int size=16;
    int16_t* data=calloc(size,sizeof(int16_t));
    for(size_t i=0;i<size;i++){
        data[i]=(int16_t)i;
    }
    struct ComplexNumber* coefficients=FFT_(data,size);
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
/// @brief  this function is trying to do FFT 
struct ComplexNumber *FFT_(int16_t *data,int size)
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
    //spliting and ordering
    FFT_spliting(coefficients,size);
    //do butterflying
    for(int b=2;b<=size;b*=2){
        //twiddle 2 then 4 then 8 then .... till it reaches size of data
        int howManyChunks=size/b;//it is like when b==2 size/2 is half which is how many pairs i need to butterfly them
        for(int chunk=0;chunk<howManyChunks;chunk++){
           for(int i=0;i<b/2;i++){
            struct ComplexNumber* twiddle=createComplexFromKN(i,b);
            struct ComplexNumber first=coefficients[i+chunk*b];
            struct ComplexNumber _=coefficients[i+b/2+chunk*b];
            struct ComplexNumber* second=multiply(&_,twiddle);
            struct ComplexNumber* addition=addTwoComplexes(&first,second);
            struct ComplexNumber* subtraction=subtractComplexes(&first,second);
            coefficients[i+chunk*b].real=addition->real;
            coefficients[i+chunk*b].imaginery=addition->imaginery;
            coefficients[i+b/2+chunk*b].real=subtraction->real;
            coefficients[i+b/2+chunk*b].imaginery=subtraction->imaginery;
            free(addition);
            free(subtraction);
            free(second);
            free(twiddle);
           }
        }
    }
    return coefficients;
}
