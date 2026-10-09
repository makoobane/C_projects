#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define TITLE "AudioViewer"

int width=-1;
int height=-1;
struct SoundBlockData{
    int16_t min;
    int16_t max;
    float rms;
};
typedef struct Screen{
SDL_Window* window;
SDL_Renderer* renderer;
float scaleOfDecibelsInPixel;
uint32_t numberOfSamples;//size of alll data
uint32_t sizeOfBlock;//how many 2bytes of subchunk2size
int16_t* block;
}Screen;


//file things
FILE* createFilePointer(const char* filename);
uint32_t read4bytes(FILE* fptr);
uint16_t read2bytes(FILE* fptr);
char* bigEndianToString(uint32_t fourBytes);
//sdl things
bool init_SDL(Screen* screen);
struct SoundBlockData findSoundBlocData(uint16_t sizeOfBlock,int16_t* block);
void drawSoundBlockData(SDL_Renderer* renderer,int xstart, int xend,struct SoundBlockData SoundBlockData);
void readBlocksAndDrawIt(FILE* fptr,Screen* screen,int xposition);
void run(FILE* fptr,Screen* screen);
void destroySDL(Screen* screen,int exit_code);
//close file
void closeFile(FILE* fptr);
int main(){
    //set up file pointer
    FILE* fptr=createFilePointer("test.wav");

    //read Riff 4bytes
    uint32_t RiffBytes=read4bytes(fptr);
    //convert big endian to Ascii
    char* RiffString=bigEndianToString(RiffBytes);
    printf("%s\n",RiffString);
    free(RiffString);
    //read size
    uint32_t wholeFileSize=read4bytes(fptr);
    printf("whole file size:%d\n",wholeFileSize);
    //read "WAVE" fourbytes
    uint32_t waveBytes=read4bytes(fptr);
    char* waveString=bigEndianToString(waveBytes);
    printf("%s\n",waveString);
    free(waveString);
    //read "fmt " string fourbytes
    uint32_t fmtBytes=read4bytes(fptr);
    char* fmtString=bigEndianToString(fmtBytes);
    printf("%s\n",fmtString);
    free(fmtString);
    //read subchunk1Size fourbytes of litte Endian
    uint32_t subchunk1Size=read4bytes(fptr);
    //I have noticed that little Endian is inverted here and read correctly
    printf("subchunk1Size:%d\n",subchunk1Size);//it is 406284
    //read audio format
    uint16_t audioFormat=read2bytes(fptr);
    printf("audioFormat:%d\n",audioFormat);//it read 1 PCM
    //read number of channels
    uint16_t numchannels=read2bytes(fptr);
    printf("number of channels are:%d\n",numchannels);//it read 1 channel(mono)
    //read frequancy
    uint32_t frequancy=read4bytes(fptr);
    printf("frequancy is :%d\n",frequancy);//frequancy is 44100HZ
    //read byteRate
    uint32_t byteRate=read4bytes(fptr);
    printf("bytereate is:%d\n",byteRate);//it is 88200
    //read blockAlign
    uint16_t blockAlign=read2bytes(fptr);
    printf("blockAlign is :%d\n",blockAlign);//it is 2 in this case
    //read BitsPerSample
    uint16_t bitsPerSample=read2bytes(fptr);
    printf("bits per sample is:%d\n",bitsPerSample);//it is 16bits per sample
    //read "data"
    uint32_t dataBytes=read4bytes(fptr);
    char* dataString=bigEndianToString(dataBytes);
    printf("%s\n",dataString);
    free(dataString);
    //read subchunk2Size of actuall data size to expect
    uint32_t actualDataSize=read4bytes(fptr);
    printf("actual data size or subchunk2Size is :%d\n",actualDataSize);// it is 406248



    
    //DISPLAYING part
    Screen _={
        .window=NULL,
        .renderer=NULL,
        .scaleOfDecibelsInPixel=0.0,
        .sizeOfBlock=0,
        .numberOfSamples=actualDataSize/(blockAlign),
        .block=NULL,
    };
    Screen* screen=&_;
    init_SDL(screen);

    run(fptr,screen);
    //close file
    closeFile(fptr);
    return 0;
}

bool init_SDL(Screen* screen)
{
 int sdlinitERr= SDL_Init(SDL_INIT_VIDEO);
 if(sdlinitERr!=0){
    fprintf(stderr,"error on sdl init:%s\n",SDL_GetError());
    return false;
}
//screen
SDL_DisplayMode display;
 int mode_error= SDL_GetDesktopDisplayMode(0,&display);
if(mode_error!=0){
    fprintf(stderr,"error at size fetching:%s\n",SDL_GetError());
    return false;
}
width=display.w;
height=display.h;



screen->window=SDL_CreateWindow(TITLE,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,0);
if(screen->window==NULL){
    fprintf(stderr,"error on sdl window creation:%s\n",SDL_GetError());
    return false;
    
}
screen->renderer=SDL_CreateRenderer(screen->window,-1,0);
if(screen->renderer==NULL){
     fprintf(stderr,"error on sdl window creation:%s\n",SDL_GetError());
     return false;
 }

 

 //how many decibels per pixel
 screen->scaleOfDecibelsInPixel=(float)65536/height;
 //size of chunk per pixel in width
 screen->sizeOfBlock=screen->numberOfSamples/width;
 //block storage
 screen->block=calloc(screen->sizeOfBlock,sizeof(int16_t));
 if(screen->block==NULL){
    puts("failed to allocate bucket of sounds those needed to draw in a pixel");
    return false;
 }
 return true;
}

struct SoundBlockData findSoundBlocData(uint16_t sizeOfBlock,int16_t* block)
{
    struct SoundBlockData soundblockData;
    int16_t min=block[0];
    int16_t max=block[0];
    uint64_t sum=0;
    for(int i=0;i<sizeOfBlock;i++){
      int16_t currentamplitude= block[i];
      if(currentamplitude>max){
        max=currentamplitude;
      }
      if(currentamplitude<min){
        min=currentamplitude;
      }
      sum+=(uint64_t)currentamplitude*currentamplitude;
    }
    soundblockData.max=max;
    soundblockData.min=min;
    soundblockData.rms=sqrtf((float)sum/sizeOfBlock);
    // printf("rms is %.2f\n",soundblockData.rms);
    return soundblockData;
}

void drawSoundBlockData(SDL_Renderer* renderer,int xstart, int xend,struct SoundBlockData soundBlockData)
{
   SDL_SetRenderDrawColor(renderer,0,0,155,15);
   int center=height/2;
   int min=center+soundBlockData.min;
   int max=center-soundBlockData.max;
   SDL_RenderDrawLine(renderer,xstart,min,xend,max);
   //draw rms 
   SDL_SetRenderDrawColor(renderer,0,255,0,255);
   int rms=(int)soundBlockData.rms+height/4;
   SDL_RenderDrawLine(renderer,xstart,rms,xstart+1,rms);// it requires reading current pixel and next pixel to draw valid line??!!
   
}

void readBlocksAndDrawIt(FILE *fptr,Screen* screen,int xposition)
{
    //reading alll of the file is somehting that iam avoiding it bcs this one is small 4s . what if i use to test several minutes audio
    //more general and consistant is needed. tell me if there is better way(claude)
    size_t howmany= fread(screen->block,sizeof(int16_t),screen->sizeOfBlock,fptr);
    // printf("%zu\n",howmany);//why it retruns 3, every time?
    //find min and max and rms
    struct SoundBlockData soundBlockData=findSoundBlocData(screen->sizeOfBlock,screen->block);
    //scale min and max
    soundBlockData.max/=screen->scaleOfDecibelsInPixel;
    soundBlockData.min/=screen->scaleOfDecibelsInPixel;
    soundBlockData.rms/=screen->scaleOfDecibelsInPixel;
    //draw it
    drawSoundBlockData(screen->renderer,xposition,xposition,soundBlockData);
}

void run(FILE* fptr,Screen* screen)
{      


         //set white color as background
        SDL_SetRenderDrawColor(screen->renderer,255,255,255,255);
        SDL_RenderClear(screen->renderer);
        //render sound 
        for(int m=0;m<width;m++){
            readBlocksAndDrawIt(fptr,screen,m);
        }
        //show horizontal line
        SDL_SetRenderDrawColor(screen->renderer,250,0,0,255);
        SDL_RenderDrawLine(screen->renderer,0,height/2,width,height/2);
        //present
        SDL_RenderPresent(screen->renderer);
        SDL_Delay(4000);

    
    
}

void destroySDL(Screen *screen,int exit_code)
{
    free(screen->block);
    SDL_DestroyRenderer(screen->renderer);
    SDL_DestroyWindow(screen->window);
    SDL_Quit();
    exit(exit_code);
}

FILE *createFilePointer(const char *filename)
{
    FILE* fptr=fopen(filename,"rb");
    if(fptr==NULL){
        puts("error on reading 4btyes");
        return NULL;
    }
    return fptr;
}

uint32_t read4bytes(FILE* fptr)
{
    uint32_t fourBytes;
    size_t four_items_read=fread(&fourBytes,sizeof(uint32_t),1,fptr);
    if(four_items_read==1){
        printf("congratulations , you have read fourbytes in this file and it is 0x%08X\n",fourBytes);
    }
    return fourBytes;
}

uint16_t read2bytes(FILE *fptr)
{
    uint16_t twobytes;
    size_t howmanyTwobytesIread=fread(&twobytes,sizeof(uint16_t),1,fptr);
    if(howmanyTwobytesIread==1){
        printf("i had read thosse two bytes:0x%04X\n",twobytes);
    }   
    return twobytes;
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

void closeFile(FILE *fptr)
{
    fclose(fptr);
}
