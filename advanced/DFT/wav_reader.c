#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>

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
    char* Riff=WV_bigEndianToString(header->chunkID);
    char* wave=WV_bigEndianToString(header->format);
    char* fmt=WV_bigEndianToString(header->subchunk1ID);
    char* data=WV_bigEndianToString(header->subchunk2ID);
    printf("WAVFILE HEADER:{chunkID:%s,\nchunk1Size:%d,\nformat:%s,\nsubchunk1ID:%s,\nsubchunk1Size:%d,\naudioFormat:%d,\nnumChannels:%d,\nsampleRate:%d,\nbyteRate:%d,\nblockAlign:%d,\nbitsperSample:%d,\nsubchunk2ID:%s,\nsubchunk2Size:%d}\n",Riff,header->cunkSize,wave,fmt,header->subchunk1Size,header->audioFormat,header->numChannels,header->sampleRate,header->byteRate,header->blockAlign,header->bitsperSample,data,header->subchunk2Size);
    free(Riff);
    free(wave);
    free(fmt);
    free(data);
}

void WV_printData(const char *filename,uint32_t subchunk2Size)
{
    size_t skipp=44;
    uint16_t KB=1024;
    FILE *fptr = fopen(filename, "rb");
    if (fptr == NULL) {
        perror("file opening failed");
        return;
    }
    
    int16_t* samples=malloc((KB/2)*sizeof(int16_t));
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
    //alignment of reading
    uint16_t howmanyKBs;
    uint16_t remainigPairBytes;
    howmanyKBs= subchunk2Size/KB;
    remainigPairBytes=subchunk2Size%KB;
    remainigPairBytes/=2;
    printf("howmanykbs:%d, remainingbytes:%d\n",howmanyKBs,remainigPairBytes);
    puts("-----------data starting-----------------------------------------------");
    for (size_t kb=0;kb<=howmanyKBs;kb++){
        fread(samples, sizeof(int16_t),KB/2, fptr);
            for(size_t i=0;i<(KB/2);i++){
                if(i==0 && kb==0){
                    printf("data:[%d,",samples[0]);
                }else if(kb==howmanyKBs && i==remainigPairBytes){
                    printf("%d]\n",samples[i]);
                    break;
                }else{
                    printf("%d,",samples[i]);
                }
            }
    }
    puts("-----------data Ended-----------------------------------------------");

    free(samples);
    fclose(fptr);
}

void WV_ViewWavFileDataMinAndMax(const char *filename)

{
    int width=-1;
    int heigth=-1;
    int sdlinitError=SDL_Init(SDL_INIT_EVERYTHING);
    if(sdlinitError!=0){
        fprintf(stderr,"error on init sdl:%s\n",SDL_GetError());
        return;
    }
    SDL_DisplayMode display;
    SDL_GetDesktopDisplayMode(0,&display);
    width=display.w;
    heigth=display.h;
    SDL_Window* window=SDL_CreateWindow("View MIN and MAX",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width, heigth,0);
    if(window==NULL){
        fprintf(stderr,"error on window creation:%s\n",SDL_GetError());
        return;
    }
    SDL_Renderer* renderer=SDL_CreateRenderer(window,-1,0);
    if(renderer==NULL){
        fprintf(stderr,"error on renderer:%s\n",SDL_GetError());
        return;
    }
    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
               case SDL_QUIT:
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                break;
            
            default:
                break;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(60);
    }
    
}

void WV_destroyWAVFileHeader(WV_WavFileHeader *header)
{
    free(header);
}
