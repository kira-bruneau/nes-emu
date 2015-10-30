#ifndef AUDIO_H
#define AUDIO_H

typedef struct Audio Audio;

Audio * audio_create(void);
void audio_destroy(Audio * audio);

int audio_start(Audio * audio);
int audio_stop(Audio * audio);
int audio_write(Audio * audio, float val);

#endif
