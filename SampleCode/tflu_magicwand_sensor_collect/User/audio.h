#ifndef __AUDIO_H__
#define __AUDIO_H__

//
// Includes
//
#include "NuMicro.h"

//
// Prototype
//
void audio_low_level_init(void);
void audio_statup_sequence(void);
void audio_setup_sample_rate(uint32_t rate);
void audio_start_play(void);

void WAVPlayer(void);
#endif  /* __AUDIO_H__ */
