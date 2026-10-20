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
    int howmanySamples=100;
    int16_t* bucket=calloc(howmanySamples,sizeof(int16_t));
    DFT_cropThoseSamples("test.wav",header->subchunk2Size,1024,1224,bucket,howmanySamples);
    free(bucket);
    
    WV_destroyWAVFileHeader(header);
    return 0;
}


