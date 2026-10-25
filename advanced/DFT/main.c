#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "wav_reader.h"
#include "DFT.h"

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
    return 0;
}


