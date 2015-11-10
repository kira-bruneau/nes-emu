#ifndef AUDIO_H
#define AUDIO_H

#include "apu/apu.h"

typedef struct Audio Audio;

int audio_init(void);
void audio_terminate(void);
Audio * audio_create(APU * apu);
void audio_destroy(Audio * audio);

int audio_start(Audio * audio);
int audio_stop(Audio * audio);

#endif
