#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wav_reader.h"

WavFileHeader* readWavFileHeader(const char *filename)
{
    size_t headerSize=sizeof(WavFileHeader);
    FILE* fptr=fopen(filename,"rb");
    if(fptr==NULL){
        puts("file reading failed");
        return NULL;
    }
    uint8_t * header=malloc(headerSize);
    fptr=fread(header,1,headerSize,fptr);
    WavFileHeader* wavHeader=malloc(sizeof(WavFileHeader));
    //copy riff chunk part
    memcpy(wavHeader->chunkID,header,4);
    memcpy(wavHeader->subchunk1Size,header+4,4);
    memcpy(wavHeader->format,header+8,4);
    //fmt subchunk
    memcpy(wavHeader->subchunk1ID,header+12,4);
    memcpy(wavHeader->subchunk1Size,header+16,4);
    memcpy(wavHeader->audioFormat,header+20,2);
    memcpy(wavHeader->numChannels,header+22,2);
    memcpy(wavHeader->sampleRate,header+24,4);
    memcpy(wavHeader->byteRate,header+28,4);
    memcpy(wavHeader->blockAlign,header+32,2);
    memcpy(wavHeader->bitsperSample,header+34,2);
    //data subchunk part copying
    memcpy(wavHeader->subchunk2ID,header+36,4);
    memcpy(wavHeader->subchunk2Size,header+40,4);
    free(header);
    fclose(fptr);
    return wavHeader;
}

char *bigEndianToString(uint32_t fourBytes)
{
    char* fourLetters=malloc(5);
    fourLetters[3] = (fourBytes >> 24) & 0xFF; 
    fourLetters[2]= (fourBytes >> 16) & 0xFF; 
    fourLetters[1]= (fourBytes >> 8)  & 0xFF; 
    fourLetters[0] = fourBytes         & 0xFF; 
    //the terminal
    fourLetters[4]='\0';
    return fourLetters;
}

void printWavHeader(WavFileHeader *header)
{
    printf("WAVFILE HEADER:{chunkID:%s|cunk1Size:%d|format:%s|subchunk1ID:%s|subchunk1Size:%d|audioFormat:%d|numChannels:%d|sampleRate%d|byteRate:%d|blockAlign:%d|bitsperSample:%d|subchunk2ID:%s|subchunk2Size:%d}",bigEndianToString(header->chunkID),header->cunkSize,bigEndianToString(header->format),bigEndianToString(header->subchunk1ID),header->subchunk1Size,header->audioFormat,header->numChannels,header->sampleRate,header->byteRate,header->blockAlign,header->bitsperSample,bigEndianToString(header->subchunk2ID),header->subchunk1Size);
}
