#ifndef DFT_H
#define DFT_H
#include <stdint.h>
void DFT_cropThoseSamples(const char* filename,uint32_t dataSize,uint32_t start, uint32_t end, int16_t* bucket,int howmanySamples);
#endif