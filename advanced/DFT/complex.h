#ifndef Complex_H
#define Complex_H
#include <stdint.h>
struct ComplexNumber
{
    double real;
    double imaginery;
};

// since those complex numbers are used for FFT then we will use e*(2*pi*k/N)i 
struct ComplexNumber* createComplexFromKN(uint32_t k, uint32_t N);
void printComplex(struct ComplexNumber* complex);
void addTwoComplexes(struct ComplexNumber* c1,struct ComplexNumber* c2);
void subtractComplexes(struct ComplexNumber* c1, struct ComplexNumber* c2);
struct ComplexNumber* multiply(struct ComplexNumber* c1, struct ComplexNumber* c2);
double findModulus(struct ComplexNumber* complex);
//this function is not needed that much since all our things are complexes but incasse
void scaleComplex(double scale, struct ComplexNumber* complex);
#endif