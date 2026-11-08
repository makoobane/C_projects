#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"

const float pi=3.141592;
struct ComplexNumber createComplexFromKN(uint32_t k, uint32_t N)
{
    float real=cos((2*pi*k)/N);
    float imaginery=sin((2*pi*k)/N)*(-1);
    struct ComplexNumber complex;
    complex.real=real;
    complex.imaginery=imaginery;
    return complex;
}


void printComplex(struct ComplexNumber complex)
{
    if(complex.imaginery>=0){
        printf("%lf+ %lfi\n",complex.real, complex.imaginery);
    }else{
        printf("%lf%lfi\n",complex.real,complex.imaginery);
    }
}
/// @brief this function adds two complexes and stores the result to first one
void addTwoComplexes(struct ComplexNumber *c1, struct ComplexNumber *c2,struct ComplexNumber* result)
{
    
    result->real=c1->real+c2->real;
    result->imaginery= c1->imaginery+c2->imaginery;
}
/// @brief this function subtracts two complexes 
void subtractComplexes(struct ComplexNumber *c1,struct  ComplexNumber *c2,struct ComplexNumber* result)
{
   
    result->real=c1->real-c2->real;
    result->imaginery= c1->imaginery-c2->imaginery;
}
/// @brief this function does common (a+bi)(c+di)=ac-bd as real and ad+bc as imaginery
void multiply(struct ComplexNumber *c1,struct  ComplexNumber *c2,struct ComplexNumber* result)
{
   
    result->real=c1->real * c2->real +(c1->imaginery*c2->imaginery)*(-1);
    result->imaginery=c1->real*c2->imaginery+ c2->real*c1->imaginery;
}

double findModulus(struct ComplexNumber *complex)
{
    return sqrtf(complex->real* complex->real +complex->imaginery* complex->imaginery);
}

void scaleComplex(float scale,struct ComplexNumber *complex)
{
    complex->real*=scale;
    complex->imaginery*=scale;
}

