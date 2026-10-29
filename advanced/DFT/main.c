#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "wav_reader.h"
#include "DFT.h"
#include "complex.h"

int main(){
    WV_WavFileHeader* header=WV_readWavFileHeader("test.wav");
    WV_printWavHeader(header);
    // WV_printData("test.wav",header->subchunk2Size);
    // WV_ViewWavFileDataMinAndMax("test.wav");
    int howmanySamples=8192;
    int16_t* bucket=calloc(howmanySamples,sizeof(int16_t));
    uint32_t start=100424;
    DFT_cropThoseSamples("test.wav",header->subchunk2Size,start,start+howmanySamples*sizeof(int16_t),bucket,howmanySamples);
    // bruteforcing DFT and printing
    double* coefficients=calloc(howmanySamples/2 +1,sizeof(double));
    double* freqs=calloc(howmanySamples/2 +1,sizeof(double));
    // // this below function is for testing to know if it makes x[1] maximum value
    // DFT_cosineFiller(bucket,howmanySamples);
    DFT_findSpectrum(bucket,howmanySamples,coefficients,freqs,header->sampleRate);
    DFT_printSpectrum(coefficients,howmanySamples/2 +1,false);
    DFT_printSpectrum(freqs,howmanySamples/2 +1,true);
    // clean things
    free(bucket);
    free(freqs);
    free(coefficients);
    
    WV_destroyWAVFileHeader(header);
    // /// for testing complexx
    // struct ComplexNumber* complex1=createComplexFromKN(3,4);
    // struct ComplexNumber* complex2=createComplexFromKN(4,8);
    // printComplex(complex1);
    // printComplex(complex2);
    // addTwoComplexes(complex1,complex2);
    // printComplex(complex1);
    // struct ComplexNumber* complex3=malloc(sizeof(struct ComplexNumber));
    // complex3->real=-3;
    // complex3->imaginery=-4.0;
    // printComplex(complex3);
    // printf("modulus of complex3 is:%lf\n",findModulus(complex3));
    // scaleComplex(0.5,complex3);
    // printComplex(complex3);
    // free(complex3);
    // free(complex1);
    // free(complex2);
    return 0;
}


