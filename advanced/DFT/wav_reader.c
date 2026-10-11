#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "wav_reader.h"

WV_WavFileHeader* WV_readWavFileHeader(const char *filename)
{
    size_t headerSize=sizeof(WV_WavFileHeader);
    FILE* fptr=fopen(filename,"rb");
    if(fptr==NULL){
        puts("ffile opening failed");
        return NULL;
    }
    uint8_t * header=malloc(headerSize);
    if(header==NULL){
        puts("header allocation failed");
        fclose(fptr);
        return NULL;
    }
    size_t h=fread(header,1,headerSize,fptr);
    if(h==headerSize){
        puts("correct ... it read first 44bytes ");
    }
    WV_WavFileHeader* wavHeader=malloc(sizeof(WV_WavFileHeader));
    if (!wavHeader) {
        puts("wav file header failed");
        free(header);
        fclose(fptr);
        return NULL;
    }
    //copy riff chunk part
    memcpy(&(wavHeader->chunkID),header,4);
    memcpy(&(wavHeader->cunkSize),header+4,4);
    memcpy(&wavHeader->format,header+8,4);
    //fmt subchunk
    memcpy(&wavHeader->subchunk1ID,header+12,4);
    memcpy(&wavHeader->subchunk1Size,header+16,4);
    memcpy(&wavHeader->audioFormat,header+20,2);
    memcpy(&wavHeader->numChannels,header+22,2);
    memcpy(&wavHeader->sampleRate,header+24,4);
    memcpy(&wavHeader->byteRate,header+28,4);
    memcpy(&wavHeader->blockAlign,header+32,2);
    memcpy(&wavHeader->bitsperSample,header+34,2);
    //data subchunk part copying
    memcpy(&wavHeader->subchunk2ID,header+36,4);
    memcpy(&wavHeader->subchunk2Size,header+40,4);
    free(header);
    fclose(fptr);
    return wavHeader;
}

char* WV_bigEndianToString(uint32_t fourBytes)
{
    char* fourLetters=malloc(5);
    fourLetters[3] = (fourBytes >> 24) & 0xFF; 
    fourLetters[2]= (fourBytes >> 16) & 0xFF; 
    fourLetters[1]= (fourBytes >> 8)  & 0xFF; 
    fourLetters[0] = fourBytes         & 0xFF; 
    fourLetters[4]='\0';
    return fourLetters;
}

void WV_printWavHeader(WV_WavFileHeader *header)
{
    printf("WAVFILE HEADER:{chunkID:%s,\nchunk1Size:%d,\nformat:%s,\nsubchunk1ID:%s,\nsubchunk1Size:%d,\naudioFormat:%d,\nnumChannels:%d,\nsampleRate:%d,\nbyteRate:%d,\nblockAlign:%d,\nbitsperSample:%d,\nsubchunk2ID:%s,\nsubchunk2Size:%d}\n",WV_bigEndianToString(header->chunkID),header->cunkSize,WV_bigEndianToString(header->format),WV_bigEndianToString(header->subchunk1ID),header->subchunk1Size,header->audioFormat,header->numChannels,header->sampleRate,header->byteRate,header->blockAlign,header->bitsperSample,WV_bigEndianToString(header->subchunk2ID),header->subchunk2Size);
}

void WV_printData(const char *filename)
{
    size_t skipp=44;
    FILE *fptr = fopen(filename, "rb");
    if (fptr == NULL) {
        perror("file opening failed");
        return;
    }
    
    uint16_t* samples=malloc(512*sizeof(uint16_t));
    if(samples==NULL){
        puts("error on allocation of samples");
        fclose(fptr);
        return;
    }
    if(fseek(fptr,skipp,SEEK_SET)!=0){
        puts("seeking failed so it it will not skipp header");
        free(samples);
        fclose(fptr);
        return;
    }
    puts("-----------data starting-----------------------------------------------");
    size_t howmanyBytesRead = fread(samples, sizeof(uint16_t),512*sizeof(uint16_t), fptr);
    if(howmanyBytesRead==1024){
        for(size_t i=0;i<512;i++){
            if(i==0){
                printf("data:[%d,",samples[0]);
            }else if(i==511){
                printf("%d]\n",samples[i]);
            }else{
                printf("%d,",samples[i]);
            }
        }
    }
    puts("-----------data Ended-----------------------------------------------");

    free(samples);
    fclose(fptr);
}

void WV_destroyWAVFileHeader(WV_WavFileHeader *header)
{
    free(header);
}
