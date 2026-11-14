#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "wav_reader.h"
#include "DFT.h"
#include "complex.h"

typedef struct { double freq; int bin; } Peak;

static void top10Peaks(const struct ComplexNumber *c, int size, double fs, Peak out[10])
{
    int half = size / 2;
    double *mag = malloc(half * sizeof *mag);
    if (!mag) { puts("magnitude allocation failed"); return; }

    for (int i = 0; i < half; i++)
        mag[i] = (double)c[i].real * c[i].real + (double)c[i].imaginery * c[i].imaginery;

    double bestMag[10];
    int    bestBin[10];
    for (int j = 0; j < 10; j++) { bestMag[j] = -1.0; bestBin[j] = -1; }

    /* skip DC (bin 0); only keep local maxima so one tone is not listed many times */
    for (int i = 1; i < half - 1; i++) {
        if (!(mag[i] > mag[i - 1] && mag[i] >= mag[i + 1])) continue;

        int j = 0;
        while (j < 10 && bestMag[j] >= mag[i]) j++;
        if (j == 10) continue;

        for (int r = 9; r > j; r--) {
            bestMag[r] = bestMag[r - 1];
            bestBin[r] = bestBin[r - 1];
        }
        bestMag[j] = mag[i];
        bestBin[j] = i;
    }

    for (int j = 0; j < 10; j++) {
        out[j].bin  = bestBin[j];
        out[j].freq = (bestBin[j] >= 0) ? bestBin[j] * fs / size : 0.0;
    }
    free(mag);
}
void FFT_printCoefficients(struct ComplexNumber* coefficients,size_t size);
struct ComplexNumber* FFT_2(int16_t* data);
void FFT_spliting(struct ComplexNumber* coefficients, int size);
void FFT_normalize(struct ComplexNumber *coefficients, int size);
struct ComplexNumber* FFT_(int16_t* data,int size);
int16_t *generateSine(double freq, double fs, int size);
int main(){
    // TESTING--------------------------------------------------------------
    // const int    N  = 4096;
    // const double fs = 44100.0;

    // /* choose a frequency that lands exactly on bin 64: 64 * fs / N = 689.0625 Hz */
    // int16_t *bucket = generateSine(689.0625, fs, N);
    // if (!bucket) return 1;
    // ACTUAL-------------------------------------------------------
    double fs=44100.0;
    WV_WavFileHeader* header=WV_readWavFileHeader("test.wav");
    WV_printWavHeader(header);
    // WV_printData("test.wav",header->subchunk2Size);
    // WV_ViewWavFileDataMinAndMax("test.wav");
    int howmanySamples=131072;
    int16_t* bucket=calloc(howmanySamples,sizeof(int16_t));
    uint32_t start=88244;
    DFT_cropThoseSamples("test.wav",header->subchunk2Size,start,start+howmanySamples*sizeof(int16_t),bucket,howmanySamples);

    struct ComplexNumber *coefficients = FFT_(bucket, howmanySamples);
    if (!coefficients) { free(bucket); return 1; }

    FFT_normalize(coefficients, howmanySamples);

    Peak top[10];
    top10Peaks(coefficients, howmanySamples, fs, top);

    puts("top ten frequencies:");
    for (int m = 0; m < 10; m++)
        printf("freq %d: %lf Hz at bin %d\n", m, top[m].freq, top[m].bin);

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
/// @brief this function will create sine waves to test fft
int16_t *generateSine(double freq, double fs, int size)
{
    int16_t *data = malloc(size * sizeof *data);
    if (!data) { puts("failed to allocate generated data"); return NULL; }
    for (int i = 0; i < size; i++) {
        double v = sin(2.0 * M_PI * freq * i / fs);
        data[i] = (int16_t)lround(v * 32767.0);
    }
    return data;
}
void FFT_normalize(struct ComplexNumber *coefficients, int size)
{
    float scale = 1.0f / (float)size;
    for (int k = 0; k < size; k++) {
        coefficients[k].real      *= scale;
        coefficients[k].imaginery *= scale;
    }
}