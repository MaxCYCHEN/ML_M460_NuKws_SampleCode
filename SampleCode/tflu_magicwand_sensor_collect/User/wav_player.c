//
// Includes
//
#include "wav_player.h"
#include "NuMicro.h"
#include "audio.h"
#include "diskio.h"
#include "ff.h"
#include <string.h>

//
// Defines
//
#define PCM_BUFFER_SIZE     (4 * 1024)

//
// Variables
//
static FIL wavFileObject;
static size_t ReturnSize;
static uint32_t s_au32WavHeader[11];
static uint32_t pmcBuffer[PCM_BUFFER_SIZE];
static size_t totalSize;
volatile uint32_t *sound = (volatile uint32_t *) HYPER_RAM_MEM_MAP;
__IO uint32_t sound_idx = 0;
//
// Functions
//

//
// wav_player_check_file
//
int8_t wav_player_check_and_copy_file(char* name)
{
    uint32_t i;
    FRESULT res;
    uint32_t u32WavSamplingRate;
    uint32_t u32StartAddr;
    
    u32StartAddr = HYPER_RAM_MEM_MAP;
    
    res = f_open(&wavFileObject, name, (FA_OPEN_EXISTING | FA_READ));
    if (res != FR_OK) {
        return -1;
    }

    //
    // Read sampling rate from WAV header
    //
    memset(s_au32WavHeader, 0, sizeof(s_au32WavHeader));
    f_read(&wavFileObject, s_au32WavHeader, 44, &ReturnSize);
    u32WavSamplingRate = s_au32WavHeader[6];

    //
    // Setup sampling rate
    //
    audio_setup_sample_rate(u32WavSamplingRate);
    printf("WAV file: sampling rate=%d\n", u32WavSamplingRate);
    
    totalSize = 0;
    while(1) {
        res = f_read(&wavFileObject, &pmcBuffer, (PCM_BUFFER_SIZE * 4), &ReturnSize);
        
        for(i = 0; i < ReturnSize; i+=4) {
            outp32((i + u32StartAddr), pmcBuffer[i]);
        }
        
        u32StartAddr += ReturnSize;
        totalSize += ReturnSize;
        
        if (f_eof(&wavFileObject)) {
            break;
        }
    }
    
    //
    // Enable MCLK
    //
    I2S_EnableMCLK(I2S0, 12000000);
    I2S0->CTL0 |= I2S_CTL0_ORDER_Msk;    
    
    NVIC_EnableIRQ(I2S0_IRQn);
    
    return 0;
}


