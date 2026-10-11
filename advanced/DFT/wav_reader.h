#ifndef WAV_FILE_READER_H
#define WAV_FILE_READER_H

#include <stdint.h>
typedef struct WV_WavFileHeader{
 //Riff chunk descriptor   
uint32_t chunkID;
uint32_t cunkSize;
uint32_t format;
//fmt subchunk
uint32_t subchunk1ID;
uint32_t subchunk1Size;
uint16_t audioFormat;
uint16_t numChannels;
uint32_t sampleRate;
uint32_t byteRate;
uint16_t blockAlign;
uint16_t bitsperSample;
//data subchunk
uint32_t subchunk2ID;
uint32_t subchunk2Size;
}WV_WavFileHeader;

WV_WavFileHeader* WV_readWavFileHeader(const char* filename);
char* WV_bigEndianToString(uint32_t fourBytes);
void WV_printWavHeader(WV_WavFileHeader* header);
void WV_printData(const char* filename);
void WV_destroyWAVFileHeader(WV_WavFileHeader* header);
#endif