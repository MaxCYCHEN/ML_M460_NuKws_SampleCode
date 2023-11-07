/*
 * Copyright (C) 2020 Arm Limited or its affiliates. All rights reserved.
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

#include "DsCnnModel.h"
#include "BufAttributes.h"

#ifdef DSCNN

#ifndef ACTIVATION_BUF_SZ
#define ACTIVATION_BUF_SZ       (0x5F00) /* default value of 128kiB */
#endif /* ACTIVATION_BUF_SZ */

static uint8_t  _tensor_arena[ACTIVATION_BUF_SZ] ALIGNMENT_ATTRIBUTE;

DsCnnModel::DsCnnModel()
{   
#ifdef NUM_MFCC_COEFFS	
    numMfccFeatures = NUM_MFCC_COEFFS;
#else
    numMfccFeatures = GetNumMfccCoeffs(); 
#endif

#ifdef FRAME_LEN	
    frameLen = FRAME_LEN;
#else
    frameLen = (static_cast<int16_t>(SAMP_FREQ * 0.001 * GetFrameLenMs()));
#endif

#ifdef FRAME_SHIFT
    frameShift = FRAME_SHIFT;
#else
    frameShift = (static_cast<int16_t>(SAMP_FREQ * 0.001 * GetFrameShiftMs()));
#endif	
    
#ifdef NUM_FRAMES
    numFrames = NUM_FRAMES;
#else
		numFrames = (static_cast<int8_t>(((1000 - GetFrameLenMs())/GetFrameShiftMs()) + 1)); // 1s window = 1000ms
#endif			
    
}

const tflite::MicroOpResolver& DsCnnModel::GetOpResolver()
{
    return this->_opResolver;
}

bool DsCnnModel::EnlistOperations()
{
#ifndef INT8_MODEL	
    this->_opResolver.AddQuantize();
#endif		
    this->_opResolver.AddReshape();
    this->_opResolver.AddConv2D();
    this->_opResolver.AddDepthwiseConv2D();
    this->_opResolver.AddAveragePool2D();
    this->_opResolver.AddFullyConnected();
    this->_opResolver.AddRelu();
    this->_opResolver.AddSoftmax();
#ifndef INT8_MODEL	
    this->_opResolver.AddDequantize();
#endif		

    return true;
}

uint8_t* DsCnnModel::GetTensorArena()
{
    return _tensor_arena;
}

size_t DsCnnModel::GetActivationBufferSize()
{
    return ACTIVATION_BUF_SZ;
}

#endif
