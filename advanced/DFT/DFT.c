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

void DFT_findSpectrum(int16_t* inputs,int N,double* coefficients,double* freqs,uint32_t fs)
{
  double mean = 0;
for (int i = 0; i < N; i++) mean += inputs[i];
mean /= N;

for (int k = 0; k <= N / 2; k++) {
    double re = 0, im = 0;
    for (int n = 0; n < N; n++) {
        double w = 0.5 - 0.5 * cos(2 * M_PI * n / (N - 1));
        double x = (inputs[n] - mean) * w;
        double ang = 2 * M_PI * ((n * k) % N) / N;
        re += x * cos(ang);
        im -= x * sin(ang);
    }
    coefficients[k]=sqrt(re*re +im*im)/N;
    double freq = (double)k * fs / N;
    freqs[k]=freq;
    
}
}

void  DFT_printSpectrum(double* coefficients, int howmanySamples,bool isFrequancy){
   if(isFrequancy){
    printf("frequancies:[");
   }else{
    printf("coefficients:[");
   }
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
