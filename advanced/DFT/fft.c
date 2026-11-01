#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "complex.h"
void FFT_printCoefficients(struct ComplexNumber* coefficients,size_t size);
struct ComplexNumber* FFT_forTwoDataPoints(int16_t* data);
void FFT_forFourDataPoints(int16_t* data, struct ComplexNumber* coefficients);
int main(){
    size_t two=2;
    int16_t* data=calloc(two,sizeof(int16_t));
    data[0]=10;
    data[1]=13;
    struct ComplexNumber* coefficients= FFT_forTwoDataPoints(data);
    FFT_printCoefficients(coefficients,two);
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
    // no spliting since it is just two
    //this is smallest butterfly
  struct ComplexNumber* coefficients=calloc(2,sizeof(struct ComplexNumber));
  if(coefficients==NULL){
    puts("it failed FFT_forTwoDataponts coeff allocation");
    return NULL;
  }
  struct ComplexNumber* data0=complexNumberFromInt16_t(data[0]);
  struct ComplexNumber* data1=complexNumberFromInt16_t(data[1]);
  struct ComplexNumber* coef1=addTwoComplexes(data0,data1);
  struct ComplexNumber* coef2=subtractComplexes(data0,data1);
  coefficients[0].real=coef1->real;
  coefficients[0].imaginery=coef1->imaginery;
  coefficients[1].real=coef2->real;
  coefficients[1].imaginery=coef2->imaginery;
   //   destroy
   free(data0);
   free(data1);
   free(coef1);
   free(coef2);
  return coefficients;
}

void FFT_forFourDataPoints(int16_t *data,struct ComplexNumber *coefficients)
{
    // split it to even and odd 
    //can we skip the spliting and doing things from head ... lets do that instead of making two arrays
    int k=4/2;
    struct ComplexNumber* twiddles=calloc(k,sizeof(struct ComplexNumber));
    for(int j=0;j<k;j++){
        struct ComplexNumber* twiddlComplex=createComplexFromKN(j,4);
        twiddles[j].real=twiddlComplex->real;
        twiddles[j].imaginery=twiddlComplex->imaginery;
        free(twiddlComplex);
    }
    

}
