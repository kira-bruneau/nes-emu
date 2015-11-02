#ifndef AUDIO_H
#define AUDIO_H

#include <stddef.h>

#define SAMPLE_RATE 44100 // Hz

typedef struct Audio Audio;

Audio * audio_create();
void audio_destroy(Audio * audio);

int audio_start(Audio * audio);
int audio_stop(Audio * audio);
size_t audio_write(Audio * audio, float val);

#endif
