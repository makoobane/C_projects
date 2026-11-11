#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "wav_reader.h"
#include "DFT.h"
#include "complex.h"
void FFT_printCoefficients(struct ComplexNumber* coefficients,size_t size);
struct ComplexNumber* FFT_2(int16_t* data);
void FFT_spliting(struct ComplexNumber* coefficients, int size);
void FFT_normalize(struct ComplexNumber *coefficients, int size);
struct ComplexNumber* FFT_(int16_t* data,int size);
double* top10Freqs(struct ComplexNumber* coefficients, int size);
int main(){
    WV_WavFileHeader* header=WV_readWavFileHeader("test.wav");
    WV_printWavHeader(header);
    // WV_printData("test.wav",header->subchunk2Size);
    // WV_ViewWavFileDataMinAndMax("test.wav");
    int howmanySamples=131072;
    int16_t* bucket=calloc(howmanySamples,sizeof(int16_t));
    uint32_t start=10000;
    DFT_cropThoseSamples("test.wav",header->subchunk2Size,start,start+howmanySamples*sizeof(int16_t),bucket,howmanySamples);
    // bruteforcing DFT and printing
    struct ComplexNumber* coefficients=FFT_(bucket,howmanySamples);
    FFT_normalize(coefficients,howmanySamples);
    
    FFT_printCoefficients(coefficients,howmanySamples);
    double* topTen= top10Freqs(coefficients,howmanySamples);
    puts("top ten frequancies are:------------");
    for(int  m=0;m<10;m++){
        printf("freq %d:%lfHz at bin %lf\n",m,topTen[m],(topTen[m]*howmanySamples/2)/44100.0);
    }
    // freeing
    free(topTen);
    free(bucket);
    free(coefficients);
    return 0;
}

void FFT_printCoefficients(struct ComplexNumber *coefficients,size_t size)
{
    printf("--------------------- complex numbers of %zu data points -------\n",size);
    for(size_t s=0;s<size;s++){
        printf("coefficients[%zu]: ",s);
        printComplex(coefficients[s]);
    }
    puts("=========================end ===");
}
struct ComplexNumber* FFT_2(int16_t* data)
{
    struct ComplexNumber* coefficients=calloc(2,sizeof(struct ComplexNumber));
    if(coefficients==NULL){
        puts("coefficients failed in two datapoints calculation");
        return NULL;
    }
    float coef1=(float)(data[0]+data[1]);
    float coef2=(float)(data[0]-data[1]);
    coefficients[0].real=coef1;
    coefficients[1].real=coef2;
    //calloc fills zeros on imaginary
    return coefficients;

}
void FFT_spliting(struct ComplexNumber *coefficients, int size)
{
    struct ComplexNumber* spareCoefficients=calloc(size,sizeof(struct  ComplexNumber));
    if(spareCoefficients==NULL){
        puts("spare allocation failed in splitting");
        return ;
    }
    int bits=(int)log2f(size);
    int reversed_index=0;
    for(int index=0;index<size;index++){
        int currentIndex=index;
        for(int b=0;b<bits;b++){
            //shift to left to make space for new binary
            reversed_index<<=1;
            //cut last binary in index and put it on reversal
            reversed_index |= (currentIndex & 1);
            //remove that last binary from index
            currentIndex >>= 1;
        }
        //now we have reversed index
        spareCoefficients[reversed_index].real=coefficients[index].real;
        spareCoefficients[reversed_index].imaginery=coefficients[index].imaginery;
        //reset reversed_index
        reversed_index=0;
    }    
    //unload those changes to real coeficients
    for(int j=0;j<size;j++){
        coefficients[j].real=spareCoefficients[j].real;
        coefficients[j].imaginery=spareCoefficients[j].imaginery;
    }
    free(spareCoefficients);
}
/// @brief  this function is trying to do FFT 
struct ComplexNumber *FFT_(int16_t *data,int size)
{

    if(size<=0|| (size & (size-1))!=0){
        puts("it is not 2 power n");
        return NULL;
    }
    struct ComplexNumber* coefficients=calloc(size,sizeof(struct ComplexNumber));
    if(coefficients==NULL){
        puts("it failed to allocate four data points coefficients");
        return NULL;
    }
    // put data in coefficients as complex
    for(size_t s=0;s<size;s++){
        coefficients[s].real=(float)data[s];
        coefficients[s].imaginery=0.0;
    }
    //spliting and ordering
    FFT_spliting(coefficients,size);
    //do butterflying
    struct ComplexNumber* second=malloc(sizeof(struct ComplexNumber));
    struct ComplexNumber* addition=malloc(sizeof(struct ComplexNumber));
    struct ComplexNumber* subtraction=malloc(sizeof(struct ComplexNumber));
    if(second==NULL|| addition==NULL||subtraction==NULL){
        puts("error ocuured on allocatting tools for fft");
        free(coefficients);
        free(second);
        free(addition);
        free(subtraction);
        return NULL;
    }
    for(int b=2;b<=size;b*=2){
        //twiddle 2 then 4 then 8 then .... till it reaches size of data
        int howManyChunks=size/b;//it is like when b==2 size/2 is half which is how many pairs i need to butterfly them
        for(int chunk=0;chunk<howManyChunks;chunk++){
            for(int i=0;i<b/2;i++){
               struct ComplexNumber twiddle=createComplexFromKN(i,b);
               struct ComplexNumber first=coefficients[i+chunk*b];
               struct ComplexNumber _=coefficients[i+b/2+chunk*b];
               multiply(&_,&twiddle,second);
               addTwoComplexes(&first,second,addition);
               subtractComplexes(&first,second,subtraction);
               coefficients[i+chunk*b].real=addition->real;
               coefficients[i+chunk*b].imaginery=addition->imaginery;
               coefficients[i+b/2+chunk*b].real=subtraction->real;
               coefficients[i+b/2+chunk*b].imaginery=subtraction->imaginery;
               //empty second, addition and subtraction
               second->real=0.0;
               second->imaginery=0.0;
               addition->real=0.0;
               addition->imaginery=0.0;
               subtraction->real=0.0;
               subtraction->imaginery=0.0;
        }
    }
}
    free(addition);
    free(subtraction);
    free(second);
    return coefficients;
}
double *top10Freqs(struct ComplexNumber *coefficients, int size)
{
    double* toptenFreq=calloc(10,sizeof(double));
    int* top10indexes=calloc(10,sizeof(int));
    if(top10indexes==NULL){
        puts("indexes container failed");
        return NULL;
    }
    if(top10Freqs==NULL){
        puts("it failed to make the bucket of top 10 freq");
        return NULL;
    }
   
    for(int i=0;i<size/2;i++){
        double maginitude=coefficients[i].real* coefficients[i].real +coefficients[i].imaginery*coefficients[i].imaginery;
        for(int j=0;j<10;j++){
          int index=top10indexes[j];
        //   use  magnitude in ffreq positions for temperory
          double magint=  toptenFreq[j];
          if(magint<maginitude){
            // move things from that position to next "propagate them"
           for(int r=index;r<9;r++){
              top10indexes[r+1]=top10indexes[r];
              toptenFreq[r+1]=toptenFreq[r];
           }
           //    then replace the current one
           top10indexes[j]=i;
           toptenFreq[j]=maginitude;
           break;
          }
        }
    }
    // convert top 10 indexes to frequancies 
    for(int i=0;i<10;i++){
        toptenFreq[i]=(double)(top10indexes[i]*44100.0/size);
    }
    free(top10indexes);
    return toptenFreq;
}
void FFT_normalize(struct ComplexNumber *coefficients, int size)
{
    float scale = 1.0f / (float)size;
    for (int k = 0; k < size; k++) {
        coefficients[k].real      *= scale;
        coefficients[k].imaginery *= scale;
    }
}