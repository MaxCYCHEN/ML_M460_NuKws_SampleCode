/*
 * Copyright (c) 2019-2022 Arm Limited. All rights reserved.
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

#pragma once

#include <array>
#include <queue>
#include <stdlib.h>
#include <string>
#include <vector>

struct TfLiteTensor;

namespace tflite {
// Forward declarations
class MicroInterpreter;
class MicroResourceVariables;
} // namespace tflite

namespace InferenceProcess {
struct DataPtr {
    void *data;
    size_t size;

    DataPtr(void *data = nullptr, size_t size = 0);

    //void invalidate();
    //void clean();

    char *begin() const;
    char *end() const;
};

struct InferenceJob {
    std::string name;
    DataPtr networkModel;
    std::vector<DataPtr> input;
    std::vector<DataPtr> output;
    std::vector<DataPtr> expectedOutput;
    size_t numBytesToPrint;
    void *externalContext;
	  uint8_t test;

    InferenceJob();
    InferenceJob(const std::string &name,
                 const DataPtr &networkModel,
                 const std::vector<DataPtr> &input,
                 const std::vector<DataPtr> &output,
                 const std::vector<DataPtr> &expectedOutput,
                 const size_t numBytesToPrint = 0,
                 void *externalContext        = nullptr);
		
		//InferenceJob(const std::string &name, const std::vector<DataPtr> &inputconst, size_t numBytesToPrint = 0);
    InferenceJob(const std::string &name, const uint8_t test);								 
     								 

    //void invalidate();
    //void clean();
};

class InferenceProcess {
public:
    InferenceProcess(uint8_t *_tensorArena, size_t _tensorArenaSize);

    bool runJob(InferenceJob &job);

private:
    static bool copyIfm(InferenceJob &job, tflite::MicroInterpreter &interpreter);
    static bool copyOfm(InferenceJob &job, tflite::MicroInterpreter &interpreter);
    static bool compareOfm(InferenceJob &job, tflite::MicroInterpreter &interpreter);
    //static void printJob(InferenceJob &job, tflite::MicroInterpreter &interpreter);
    //static void printOutputTensor(TfLiteTensor *output, size_t bytesToPrint);
    static void tfluDebugLog(const char *s);

    uint8_t *tensorArena;
    const size_t tensorArenaSize;
};
} // namespace InferenceProcess
