#ifndef DFT_H
#define DFT_H
#include <stdint.h>
#include <stdbool.h>
// crops some samples from wav file
void DFT_cropThoseSamples(const char* filename,uint32_t dataSize,uint32_t start, uint32_t end, int16_t* bucket,int howmanySamples);
// finds fT of given inputs and stores as coefficents
void DFT_findSpectrum(int16_t* inputs,int N,double* coefficients,double* freqs,uint32_t fs);
// prints spectrum
void  DFT_printSpectrum(double* coefficients, int howmanySamples,bool isFrequancy);
// fills bucket of cosine cos(x) fucntion for testing
void DFT_cosineFiller(int16_t* cosineSamples, int howmanySamples);
#endif