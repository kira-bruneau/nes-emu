#include <stdlib.h>
#include <string.h>

#include <portaudio.h>

#include "audio.h"
#include "../struct/buffer.h"

#define BUFFER_SIZE 1024

struct Audio {
  PaStream * stream;
  Buffer * buffer;
};

static int audio_callback(const void * input_buffer,
                          void * output_buffer,
                          unsigned long frames_per_buffer,
                          const PaStreamCallbackTimeInfo * time_info,
                          PaStreamCallbackFlags status_flags,
                          void * user_data) {
  (void)input_buffer;
  (void)time_info;
  (void)status_flags;
  (void)user_data;

  float * out = (float *)output_buffer;
  Audio * audio = (Audio *)user_data;

  int num_read = 0;
  if (audio->buffer != NULL) {
    num_read = buffer_read(audio->buffer, out, frames_per_buffer);
  }

  memset(out, 0, sizeof(float) * (frames_per_buffer - num_read));
  return 0;
}

Audio * audio_create(void) {
  PaError err;
  err = Pa_Initialize();
  if (err != paNoError) {
    return NULL;
  }

  Audio * audio = malloc(sizeof(Audio));
  audio->stream = NULL;
  audio->buffer = NULL;

  err = Pa_OpenDefaultStream(&audio->stream,
                             0, // input channels (none)
                             1, // output channels (mono)
                             paFloat32,
                             SAMPLE_RATE,
                             paFramesPerBufferUnspecified,
                             audio_callback,
                             audio);

  if (err != paNoError) {
    audio_destroy(audio);
    return NULL;
  }

  return audio;
}

void audio_destroy(Audio * audio) {
  audio_stop(audio);
  Pa_CloseStream(audio->stream);
  Pa_Terminate();
  free(audio);
}

int audio_start(Audio * audio) {
  if (audio->buffer != NULL) {
    return 0;
  }

  PaError err = Pa_StartStream(audio->stream);
  if (err == paNoError) {
    audio->buffer = buffer_create(sizeof(float), BUFFER_SIZE);
    return audio->buffer != NULL;
  }

  return 0;
}

int audio_stop(Audio * audio) {
  if (audio->buffer == NULL) {
    return 0;
  }

  PaError err = Pa_StopStream(audio->stream);
  free(audio->buffer);
  audio->buffer = NULL;

  return err == paNoError;
}

int audio_write(Audio * audio, float val) {
  if (audio->buffer == NULL) {
    return 0;
  }

  return buffer_write(audio->buffer, &val, 1);
}
