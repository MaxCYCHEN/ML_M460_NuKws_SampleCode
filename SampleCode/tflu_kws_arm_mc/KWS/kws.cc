/*
 * Copyright (C) 2021 Arm Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Description: Keyword spotting example code using MFCC feature extraction
 * and neural network.
 */

#include "kws.h"
#include <float.h>

KWS::KWS(int recordWindow, int slidingWinLen)
{
    if (this->_InitModel()) {
        this->recordingWin = recordWindow;
        this->slidingWindowLen = slidingWinLen;
        this->InitKws();
    }
}

KWS::KWS(int16_t * ptrAudioBuffer, uint32_t nElements)
{
    if (this->_InitModel()) {
        this->audioBuffer = std::move(std::vector<int16_t>(
                                            ptrAudioBuffer,
                                            ptrAudioBuffer + nElements));
        this->recordingWin = model->GetNumFrames();
        this->slidingWindowLen = 1;
        this->InitKws();
    }
}

void KWS::InitKws()
{
    if (!model->IsInited()) {
        printf("Warning: model has not been initialised\r\n");
        model->Init();
    }

    numMfccFeatures = model->GetNumMfccFeatures();
    numFrames = model->GetNumFrames();
    frameLen = model->GetFrameLen();
    frameShift = model->GetFrameShift();
    numOutClasses = model->GetOutputShape()->data[1];  // Output shape should be [1, numOutClasses].

    // Following are for debug purposes.
    printf("Initialising KWS object..\r\n");
    printf("numMfccFeatures: %d\r\n", numMfccFeatures);
    printf("numFrames: %d\r\n", numFrames);
    printf("frameLen: %d\r\n", frameLen);
    printf("frameShift: %d\r\n", frameShift);
    printf("numOutClasses: %d\r\n", numOutClasses);

    //mfcc =  std::unique_ptr<MFCC>(new MFCC(numMfccFeatures, frameLen));
		mfcc =  new MFCC(numMfccFeatures, frameLen);
    mfccBuffer = std::vector<float>(numFrames * numMfccFeatures, 0.0);
    output = std::vector<float>(numOutClasses, 0.0);
    averagedOutput = std::vector<float>(numOutClasses, 0.0);
    predictions = std::vector<float>(slidingWindowLen * numOutClasses, 0.0);
    audioBlockSize = recordingWin * frameShift;
    audioBufferSize = audioBlockSize + frameLen - frameShift;
		
		/*
        Get input and output quantization information
    */
    TfLiteTensor *outputTensor = model->GetOutputTensor();
    TfLiteTensor *inputTensor = model->GetInputTensor();

    /* 
        Vww model preprocessing is image conversion from uint8 to [0,1] float values,
        then quantize them with input quantization info.  
    */
    inQuantParams = GetTensorQuantParams(inputTensor);
    outQuantParams = GetTensorQuantParams(outputTensor);
		
}

void KWS::ExtractFeatures()
{
    if (numFrames > recordingWin) {
        // Move old features left.
        memmove(mfccBuffer.data(),
                mfccBuffer.data() + (recordingWin * numMfccFeatures),
                (numFrames - recordingWin) * numMfccFeatures * sizeof(float));
    }
    // Compute features only for the newly recorded audio.
    int32_t mfccBufferHead = (numFrames - recordingWin) * numMfccFeatures;
    for (uint16_t f = 0; f < recordingWin; f++) {
        mfcc->MfccCompute(audioBuffer.data() + (f * frameShift), &mfccBuffer[mfccBufferHead]);
        mfccBufferHead += numMfccFeatures;
    }
}

void KWS::Classify()
{

#ifdef INT8_MODEL	
	  // Int8 ver. Copy mfcc features into the TfLite tensor.
	  int8_t* inTensorData = tflite::GetTensorData<int8_t>(model->GetInputTensor());
	  
	  __attribute__((aligned))int8_t mfccBuffer_int8[numFrames * numMfccFeatures * sizeof(int8_t)];
	  uint32_t ct;
	  for(ct = 0; ct < (numFrames * numMfccFeatures); ct++){
	      mfccBuffer_int8[ct] = static_cast<int8_t>((mfccBuffer[ct]/ inQuantParams.scale) + inQuantParams.offset);
	  }
		// C++ 11 transform + lambda 
	  //std::transform(mfccBuffer.begin(), mfccBuffer.end(), mfccBuffer_int8, [this] (float x) { return static_cast<int8_t>((x/ inQuantParams.scale) + inQuantParams.offset); });
	  memcpy(inTensorData, mfccBuffer_int8, numFrames * numMfccFeatures * sizeof(int8_t));
		
		// Run inference on this data.
    model->RunInference();

    // Get output from the TfLite tensor.
    int8_t* outTensorData = tflite::GetTensorData<int8_t>(model->GetOutputTensor());
		
		for(ct = 0; ct < (numOutClasses); ct++){
	      output[ct] = static_cast<float>(outQuantParams.scale*(outTensorData[ct] - outQuantParams.offset));
	  }
	
#else
    // Copy mfcc features into the TfLite tensor.
    float* inTensorData = tflite::GetTensorData<float>(model->GetInputTensor());
    memcpy(inTensorData, mfccBuffer.data(), numFrames * numMfccFeatures * sizeof(float));

    // Run inference on this data.
    model->RunInference();

    // Get output from the TfLite tensor.
    float* outTensorData = tflite::GetTensorData<float>(model->GetOutputTensor());
    memcpy(output.data(), outTensorData, numOutClasses * sizeof(float));
#endif			
}

int KWS::GetTopClass(const std::vector<float>& prediction)
{
    int maxInd = 0;
    float maxVal = FLT_MIN;
    for (int i = 0; i < numOutClasses; i++) {
        if (maxVal < prediction[i]) {
            maxVal = prediction[i];
            maxInd = i;
        }
    }
    return maxInd;
}

void KWS::AveragePredictions()
{
    // Shift the old predictions left.
    memmove(predictions.data(),
            predictions.data() + numOutClasses,
            (slidingWindowLen - 1) * numOutClasses * sizeof(float));

    // Add new predictions at the end.
    memmove((predictions.data() + (slidingWindowLen - 1 ) * numOutClasses),
            output.data(),
            numOutClasses * sizeof(float));

    // Compute averages.
    float sum;
    for (int j = 0; j < numOutClasses; j++) {
        sum = 0;
        for(int i = 0; i < slidingWindowLen; i++) {
            sum += predictions[i*numOutClasses + j];
        }
        averagedOutput[j] = (sum / slidingWindowLen);
    }
}

QuantParams KWS::GetTensorQuantParams(TfLiteTensor* tensor)
{
    QuantParams params;
    if (kTfLiteAffineQuantization == tensor->quantization.type) 
    {
        auto* quantParams = (TfLiteAffineQuantization*) (tensor->quantization.params);
        if (quantParams && 0 == quantParams->quantized_dimension) 
        {
            if (quantParams->scale->size) 
            {
                params.scale = quantParams->scale->data[0];
            }
            if (quantParams->zero_point->size) 
            {
                params.offset = quantParams->zero_point->data[0];
            }
        } 
        else if (tensor->params.scale != 0.0) 
        {
            /* Legacy tensorflow quantisation parameters */
            params.scale = tensor->params.scale;
            params.offset = tensor->params.zero_point;
        }
    }
    return params;
}