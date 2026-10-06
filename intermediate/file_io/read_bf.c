#include <stdio.h>
#include <stdint.h>

#include <stdlib.h>

FILE* createFilePointer(const char* filename);
uint32_t read4bytes(FILE* fptr);
uint16_t read2bytes(FILE* fptr);
char* bigEndianToAscii(uint32_t fourBytes);
void closeFile(FILE* fptr);
int main(){
    //set up file pointer
    FILE* fptr=createFilePointer("test.wav");

    //read Riff 4bytes
    uint32_t RiffBytes=read4bytes(fptr);
    //convert big endian to Ascii
    char* RiffString=bigEndianToAscii(RiffBytes);
    printf("%s\n",RiffString);
    free(RiffString);
    //read size
    uint32_t wholeFileSize=read4bytes(fptr);
    printf("%d\n",wholeFileSize);
    //read "WAVE" fourbytes
    uint32_t waveBytes=read4bytes(fptr);
    char* waveString=bigEndianToAscii(waveBytes);
    printf("%s\n",waveString);
    free(waveString);
    //read "fmt " string fourbytes
    uint32_t fmtBytes=read4bytes(fptr);
    char* fmtString=bigEndianToAscii(fmtBytes);
    printf("%s\n",fmtString);
    free(fmtString);
    //read subchunk1Size fourbytes of litte Endian
    uint32_t subchunk1Size=read4bytes(fptr);
    //I have noticed that little Endian is inverted here and read correctly
    printf("subchunk1Size:%d\n",subchunk1Size);//it is 406248
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
    char* dataString=bigEndianToAscii(dataBytes);
    printf("%s\n",dataString);
    free(dataString);
    //read subchunk2Size of actuall data size to expect
    uint32_t actualDataSize=read4bytes(fptr);
    printf("actual data size or subchunk2Size is :%d\n",actualDataSize);// it is 406248, but why it is same as subchunk1Size??




    //close file
    closeFile(fptr);
    return 0;
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

char *bigEndianToAscii(uint32_t fourBytes)
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
