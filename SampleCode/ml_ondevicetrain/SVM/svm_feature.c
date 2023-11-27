#include "svm_feature.h"


//-------------------------------------------------------------//
/*
	Function Brief: To Initial a Buffer with initial values.
*/
//-------------------------------------------------------------//
void SVM_DataBufferReset(DATABUFFER* spbuffer)
{
    spbuffer->idx = 0;

    spbuffer->stat.max = -32768;//0;
    spbuffer->stat.min = 32767;//65535;
    spbuffer->stat.average = 0;
    spbuffer->stat.average_buf = 0;
    spbuffer->stat.amplitude = 0;
    spbuffer->stat.sum = 0;
    spbuffer->stat.sum_square = 0;
    spbuffer->stat.variance = 0.0;

}

//-------------------------------------------------------------//
/*
	Function Brief: To Check it ths Buffer is filled with data.
*/
//-------------------------------------------------------------//
bool SVM_DataBufferIsFull(DATABUFFER* spbuffer, int size)
{
    if ((spbuffer->idx+1) > size) return true; //sbuffer is full
    else return false;
}


//-------------------------------------------------------------------//
/*
	To update the Statistic Feature when desired Queue Size is full
	Feature and unit are summary as follows:
	max: LSB
	min: LSB
	average: LSB
	maxima: pysical quantity.Degree/Sec(yaw-rate) or mili g(longitude acc)
	sgnmaxima : signed pysical quantity.Degree/Sec(yaw-rate) or mili g(longitude acc)
	peak: pysical quantity.Degree/Sec(yaw-rate) or mili g(longitude acc)
	variance(standard deviation): pysical quantity, Degree/Sec or mili g
*/
//-------------------------------------------------------------------//
bool SVM_DataBufferUpdateFeature(DATABUFFER *spbuffer, float data)//, STATFEATURE *spimufeature);
{

    if (SVM_DataBufferProcessDone(spbuffer, data) == true)
    {
        SVM_DataBufferReset(spbuffer);
        return true;

    }
    else return false;
}


//-------------------------------------------------------------//
/*
	Function Brief: To Check it ths Buffer is filled with data.
*/
//-------------------------------------------------------------//
bool SVM_DataBufferProcessDone(DATABUFFER* spbuffer, float data)
{

    if(SVM_DataBufferIsFull(spbuffer, BSIZE)==true)//sbuffer is full
    {
        spbuffer->stat.average = spbuffer->stat.average_buf / BSIZE;
        spbuffer->stat.average_buf = 0;
        spbuffer->stat.amplitude = spbuffer->stat.max - spbuffer->stat.min;
        spbuffer->idx = 0;
        spbuffer->stat.variance = spbuffer->stat.sum_square - (((float)(spbuffer->stat.sum * spbuffer->stat.sum))/ BSIZE);
        spbuffer->stat.variance /= (BSIZE - 1);
        //spbuffer->stat.variance = sqrtf(spbuffer->stat.variance);
			  //printf("%d,%f\r\n",spbuffer->stat.amplitude, 10000*spbuffer->stat.variance);
			  spbuffer->stat.output_amplitude  = spbuffer->stat.amplitude; 
			  spbuffer->stat.output_variance  = spbuffer->stat.variance; 
        return true;
    }
    else//sbuffer is not full
    {
        spbuffer->idx++;
        spbuffer->stat.average_buf += data;
        //spbuffer->stat.sum += (data/ (spbuffer->stat.gain* spbuffer->stat.gain));
        spbuffer->stat.sum += (data / (spbuffer->stat.gain));
        spbuffer->stat.sum_square += (data*data/(spbuffer->stat.gain * spbuffer->stat.gain));
        if (data > spbuffer->stat.max) spbuffer->stat.max = data;
        if (data < spbuffer->stat.min) spbuffer->stat.min = data;
        return false;
    }

}



//-------------------------------------------------------------//
/*
	Function Brief: To Set Sensor Gain with correct values.
	YAW LSB->DPS:114
	ACC LSB->MG:4
*/
//-------------------------------------------------------------//
void SVM_DataBufferSetSensorGain(DATABUFFER* spbuffer, float gain)
{
    spbuffer->stat.gain = gain;

}

void SVM_DataBufferSetAxisIndex(DATABUFFER* spbuffer, uint8_t index)
{
    spbuffer->stat.axis_index = index;

}



void SVM_ScaleOutput(int index, float value)
{
	/* skip single-valued attribute */
//	if(feature_max[index] == feature_min[index])
//		return;

//	if(value == feature_min[index])
//		value = lower;
//	else if(value == feature_max[index])
//		value = upper;
//	else
//		value = lower + (upper-lower) *
//			(value-feature_min[index])/
//			(feature_max[index]-feature_min[index]);

//	if(value != 0)
//	{
//		printf("%d:%g ",index, value);
//	}
}