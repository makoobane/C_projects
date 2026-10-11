#include <stdio.h>
#include "wav_reader.h"

int main(){
    WV_WavFileHeader* header=WV_readWavFileHeader("test.wav");
    WV_printWavHeader(header);
    WV_printData("test.wav");
    WV_destroyWAVFileHeader(header);
    return 0;
}


