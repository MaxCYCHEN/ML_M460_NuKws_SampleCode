

#ifndef SVM_FEATURE_H
#define SVM_FEATURE_H
#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define BSIZE 128//256
#define YAW_DPSFACTOR 114
#define ACC_MGFACTOR 4
#define YAW_STABLETHRESHOLD


typedef struct
{
    uint8_t axis_index;
	  int32_t max;
    int32_t min;
    int32_t amplitude;
    float average;
    float average_buf;
    float sum;
    float sum_square;
    float variance;
    float peak;
    float gain;
	  float output_amplitude;
	  float output_variance;
} STATFEATURE;

typedef struct
{
    int idx;
    int buffer[BSIZE];
    STATFEATURE stat;
} DATABUFFER;


void SVM_DataBufferReset(DATABUFFER* spbuffer);
bool SVM_DataBufferIsFull(DATABUFFER* spbuffer, int size);
bool SVM_DataBufferProcessDone(DATABUFFER* spbuffer, float data);
bool SVM_DataBufferUpdateFeature(DATABUFFER *spbuffer, float data);
void SVM_DataBufferSetSensorGain(DATABUFFER* spbuffer, float gain);
void SVM_DataBufferSetAxisIndex(DATABUFFER* spbuffer, uint8_t indexgain);


/*****************************************************************************/
#ifdef __cplusplus
}	/* extern */
#endif

#endif
