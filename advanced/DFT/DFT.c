#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
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
  if(fseek(fptr,start+skipp,SEEK_SET)!=0){
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

void DFT_findSpectrum_bruteForce(int16_t *inputs, int howmanySamples, float *coefficients)
{
  // x[kr]=(1/howmanySamples)*sum(x[n].cos((2*pi*n*k)/howmanySamples)) --->real part or cosine part
  // x[ki]=(1/howmanySamples)*sum(x[n].sin((2*pi*n*k)/howmanySamples)) -----> imaginery part or cosine part
  // final x[k]=sqrt(x[kr]*x[kr]+x[ki]*x[ki])
  // I saw that complex.h library can offer cexp(complex) and can do this also creal and cimag functions could do this 
  // in better but cosine and sine is more pleasant
  float normal=(float)1/howmanySamples;
  const float PI=3.141592;
  for (int k=0;k<howmanySamples;k++){
      float xkReal=0.0;
      float xkImag=0.0;
      for(int n=0;n<howmanySamples;n++){
          int16_t xOfn=inputs[n];
          int idx = (n*k) % howmanySamples;
          float cosnk=cos(2*PI*idx/howmanySamples);
          float sinnk=sin(2*PI*idx/howmanySamples);
          xkReal+=(xOfn*cosnk);
          xkImag-=(xOfn*sinnk);
      }
      xkReal*=normal;
      xkImag*=normal;
      float xk=sqrtf(xkReal*xkReal+xkImag*xkImag);
      coefficients[k]=xk;
  }
}

void  DFT_printSpectrum(float* coefficients, int howmanySamples){
   printf("spectrum:[");
   for(int j=0;j<howmanySamples;j++){
    if(j==0&& howmanySamples!=1){
      printf("%f",coefficients[j]);
    }else if(j==howmanySamples-1){
      printf(",%f]\n",coefficients[j]);
    }else{
      printf(",%f",coefficients[j]);
    }
   }
}

void DFT_cosineFiller(int16_t *cosineSamples, int howmanySamples)
{
   float stepp=(M_PI*2)/howmanySamples;
   int max=32767;
   for(int i=0;i<howmanySamples;i++){
    float x=stepp*i;
    float cosv=cos(x);
    int16_t cosineSample=(int16_t)(cosv*max);
    cosineSamples[i]=cosineSample;
   }
}
