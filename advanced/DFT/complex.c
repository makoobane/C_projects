#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"

const double pi=3.141592654;
struct ComplexNumber *createComplexFromKN(uint32_t k, uint32_t N)
{
    struct ComplexNumber* complex=malloc(sizeof(struct ComplexNumber));
    if(complex==NULL){
        puts("allocation of creation complex Number from K and N failed");
        return NULL;
    }
    double real=cos((2*pi*k)/N);
    double imaginery=sin((2*pi*k)/N)*(-1);
    complex->real=real;
    complex->imaginery=imaginery;
    return complex;
}

void printComplex(struct ComplexNumber *complex)
{
    if(complex->imaginery>0){
        printf("%lf+ %lfi\n",complex->real, complex->imaginery);
    }else{
        printf("%lf%lfi\n",complex->real,complex->imaginery);
    }
}
/// @brief this function adds two complexes and stores the result to first one
/// @param c1 is complex one which the result will be stored
void addTwoComplexes(struct ComplexNumber *c1, struct ComplexNumber *c2)
{
    c1->real+=c2->real;
    c1->imaginery+=c2->imaginery;
}
/// @brief this function subtracts two complexes 
/// @param c1 complex one is where result is stored
void subtractComplexes(struct ComplexNumber *c1,struct  ComplexNumber *c2)
{
    c1->real-=c2->real;
    c1->imaginery-=c2->imaginery;
}
/// @brief this function does common (a+bi)(c+di)=ac-bd as real and ad+bc as imaginery
struct ComplexNumber *multiply(struct ComplexNumber *c1,struct  ComplexNumber *c2)
{
    struct ComplexNumber* result=malloc(sizeof(struct ComplexNumber));
    if(result==NULL){
        puts("it failed to allocate complex number in multiply function");
        return NULL;
    }
    result->real=c1->real * c2->real +(c1->imaginery*c2->imaginery)*(-1);
    result->imaginery=c1->real*c2->imaginery+ c2->real*c1->imaginery;
    return result;
}

double findModulus(struct ComplexNumber *complex)
{
    return sqrt(complex->real* complex->real +complex->imaginery* complex->imaginery);
}

void scaleComplex(double scale,struct ComplexNumber *complex)
{
    complex->real*=scale;
    complex->imaginery*=scale;
}
