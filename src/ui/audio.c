#include <stdlib.h>
#include <string.h>

#include <portaudio.h>
#include <glib.h>

#include "audio.h"
#include "../struct/buffer.h"

#define SAMPLE_RATE 44100
#define BUFFER_SIZE 44100

struct Audio {
  PaStream * stream;
  Buffer * buffer;
  GMutex io_mutex;
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

  if (audio->buffer == NULL) {
    memset(out, 0.0f, frames_per_buffer * sizeof(float));
    return 0;
  }

  g_mutex_lock(&audio->io_mutex);

  unsigned long i;
  for (i = 0; i < frames_per_buffer; ++i, ++out) {
    float val = 0.0f;
    buffer_read(audio->buffer, &val, sizeof(float), 1);
    *out = val;
  }

  g_mutex_unlock(&audio->io_mutex);

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
    g_mutex_init(&audio->io_mutex);
    audio->buffer = buffer_create(BUFFER_SIZE);
    return audio->buffer != NULL;
  }

  return 0;
}

int audio_stop(Audio * audio) {
  if (audio->buffer == NULL) {
    return 0;
  }

  PaError err = Pa_StopStream(audio->stream);
  g_mutex_clear(&audio->io_mutex);
  free(audio->buffer);
  audio->buffer = NULL;

  return err == paNoError;
}

int audio_write(Audio * audio, float val) {
  if (audio->buffer == NULL) {
    return 0;
  }

  int result;
  g_mutex_lock(&audio->io_mutex);
  result = buffer_write(audio->buffer, &val, sizeof(float), 1);
  g_mutex_unlock(&audio->io_mutex);

  return result;
}
