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
    remainigPairBytes/=sizeof(int16_t);
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
    const char* TITLE="MIN and MAX of WAV File data Viewer";
    //sdl inititalization...
    int sdlinitError=SDL_Init(SDL_INIT_EVERYTHING);
    if(sdlinitError!=0){
        fprintf(stderr,"error on init sdl:%s\n",SDL_GetError());
        return;
    }
    SDL_DisplayMode display;
    SDL_GetDesktopDisplayMode(0,&display);
    width=display.w;
    heigth=display.h;
    SDL_Window* window=SDL_CreateWindow(TITLE,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width, heigth,0);
    if(window==NULL){
        fprintf(stderr,"error on window creation:%s\n",SDL_GetError());
        return;
    }
    SDL_Renderer* renderer=SDL_CreateRenderer(window,-1,0);
    if(renderer==NULL){
        fprintf(stderr,"error on renderer:%s\n",SDL_GetError());
        return;
    }
    //read data
    size_t skipp=40;
    FILE *fptr = fopen(filename, "rb");
    if (fptr == NULL) {
        perror("file opening failed");
        return;
    }
    if(fseek(fptr,skipp,SEEK_SET)!=0){
        puts("seeking failed");
        fclose(fptr);
        return;
    }
    uint32_t dataSize;
    size_t dataSizeReadIs= fread(&dataSize,1,sizeof(uint32_t),fptr);
    if(dataSizeReadIs!=4){
        printf("failed to read file data size,%zu\n",dataSizeReadIs);
        fclose(fptr);
        return;
    }
    printf("data size is:%d bytes\n",dataSize);
    int numberOfSamples=(dataSize/sizeof(int16_t));
    printf("number of samples where each is 16bits is :%d and width is:%d\n",numberOfSamples,width);
    int samplesPerPixelWidth=numberOfSamples/width;
    int remainingSamples=numberOfSamples%width;
    printf("samples in pixel avg was:%d, and remainedSamples was:%d\n",samplesPerPixelWidth,remainingSamples);
    //distribute the remaining by increasing howmany samples in each pixel then some pixels of width would be jobless
    samplesPerPixelWidth+=1;
    int drawableWidth=width - (width-remainingSamples)/samplesPerPixelWidth;
    printf("drawable width:%d\n",drawableWidth);
    //this is where we store each block of samples to find their min  and max 
    int16_t* samplesBlock=calloc(samplesPerPixelWidth,sizeof(int16_t));
    //this is where we will store min and max for each drawablewidth
    int16_t* maxAndMinOfEachBlockSamples=calloc(drawableWidth*2,sizeof(int16_t));
    //read each block , find min and max and store them in "maxAndMinOfEachBlockSamples" array
    for(int pixel=0;pixel<drawableWidth;pixel++){
      size_t bytesRead= fread(samplesBlock,1,samplesPerPixelWidth*sizeof(int16_t),fptr);
      if(bytesRead==samplesPerPixelWidth*sizeof(int16_t)){
        //find min and max
        int16_t min=samplesBlock[0];
        int16_t max=samplesBlock[0];
        for(int i=0;i<samplesPerPixelWidth;i++){
            if(samplesBlock[i]>max){
                max=samplesBlock[i];
            }
            if(samplesBlock[i]<min){
                min=samplesBlock[i];
            }
        }
        //store min and max min and max array
        maxAndMinOfEachBlockSamples[pixel*2]=min;
        maxAndMinOfEachBlockSamples[pixel*2 + 1]=max;
      }
    }
    free(samplesBlock);
    samplesBlock=NULL;
    //prepare to draw "maxAndMinOfEachBlockSamples"
    int scaleOfLoudnessPerPixel=32768/(heigth/2);
    int centery=heigth/2;
    //close file
    fclose(fptr);
    //window loop
    bool running =true;
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running=false;
                break;
           
            default:
                break;
            }
        }
        SDL_RenderClear(renderer);
        for(int p=0;p<drawableWidth;p++){
            SDL_SetRenderDrawColor(renderer,0,0,200,255);//blue
            int min=maxAndMinOfEachBlockSamples[2*p]/scaleOfLoudnessPerPixel;
            int max=maxAndMinOfEachBlockSamples[2*p+1]/scaleOfLoudnessPerPixel;
            SDL_RenderDrawLine(renderer,p,centery+min,p,centery+max);//doubtt
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(160);
        if(running==false){
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
    }
}

void WV_destroyWAVFileHeader(WV_WavFileHeader *header)
{
    free(header);
}
