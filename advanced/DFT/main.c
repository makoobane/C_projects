#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "wav_reader.h"
#include "DFT.h"

int main(){
    WV_WavFileHeader* header=WV_readWavFileHeader("test.wav");
    WV_printWavHeader(header);
    // WV_printData("test.wav",header->subchunk2Size);
    // WV_ViewWavFileDataMinAndMax("test.wav");
    int howmanySamples=200;
    int16_t* bucket=calloc(howmanySamples,sizeof(int16_t));
    DFT_cropThoseSamples("test.wav",header->subchunk2Size,1024,1424,bucket,howmanySamples);
    // bruteforcing DFT and printing
    float* coefficients=calloc(howmanySamples,sizeof(float));
    // // this below function is for testing to know if it makes x[1] maximum value
    // DFT_cosineFiller(bucket,howmanySamples);
    DFT_findSpectrum_bruteForce(bucket,howmanySamples,coefficients);
    DFT_printSpectrum(coefficients,howmanySamples);
    // clean things
    free(bucket);
    free(coefficients);
    
    WV_destroyWAVFileHeader(header);
    return 0;
}


