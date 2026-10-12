#include <stdio.h>
#include "wav_reader.h"

int main(){
    WV_WavFileHeader* header=WV_readWavFileHeader("test.wav");
    WV_printWavHeader(header);
    WV_printData("test.wav",header->subchunk2Size);
    WV_ViewWavFileDataMinAndMax("test.wav");
    WV_destroyWAVFileHeader(header);
    return 0;
}


