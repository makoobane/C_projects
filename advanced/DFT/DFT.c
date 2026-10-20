#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "DFT.h"

// this function is primarly made for wav file with sample size of 16bits
void DFT_cropThoseSamples(const char *filename,uint32_t dataSize, uint32_t start, uint32_t end, int16_t *bucket,int howmanySamples)
{
  //befor any thing check if start and end are correct with bucket size
  bool correctLimits=start<dataSize && end<=dataSize && howmanySamples*sizeof(int16_t)==(end-start);
  if(correctLimits==false){
   puts("check your limits of start, end , sizeofbucket(howmanySamples)  and data size ... they should obey: datasize>start and end.. also end-start==sizeOfBucket");
   return;
  }
  FILE* fptr=fopen(filename,"rb");
  if(fptr==NULL){
    printf("%s file opening failed\n",filename);
    return;
  }
//   skip the header
  size_t skipp=44;
  if(fseek(fptr,skipp,SEEK_SET)!=0){
        puts("seeking failed so it it will not skipp header");
        fclose(fptr);
        return;
  }
//   skip to start
  if(fseek(fptr,start,SEEK_SET)!=0){
    puts("seeking to starting point failed");
    fclose(fptr);
    return;
  }
// read between start and stop
size_t samplesRead= fread(bucket,sizeof(int16_t),howmanySamples,fptr);

if(samplesRead==howmanySamples){
    printf("correctly read %zu bytes equavalent to %d samples \n",samplesRead*sizeof(int16_t),howmanySamples);
}
  fclose(fptr);
}