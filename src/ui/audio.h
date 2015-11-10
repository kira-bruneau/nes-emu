#ifndef AUDIO_H
#define AUDIO_H

#include <stddef.h>

typedef struct NES NES;
typedef struct Audio Audio;

Audio * audio_create(NES * nes);
void audio_destroy(Audio * audio);

int audio_start(Audio * audio);
int audio_stop(Audio * audio);

#endif
