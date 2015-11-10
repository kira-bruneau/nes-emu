#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <portaudio.h>
#include <glib.h>

#include "audio.h"

#define SAMPLE_RATE 44100

struct Audio {
  PaStream * stream;
  APU * apu;
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

  float * out = (float *)output_buffer;
  Audio * audio = (Audio *)user_data;

  unsigned long i = 0;
  for (i = 0; i < frames_per_buffer; ++i) {
    *out++ = apu_sample(audio->apu);
  }

  return 0;
}

Audio * audio_create(APU * apu) {
  PaDeviceIndex device = Pa_GetDefaultOutputDevice();
  if (device == paNoDevice) {
    return NULL;
  }

  PaStreamParameters output_parameters = {
    .channelCount = 1,
    .device = device,
    .hostApiSpecificStreamInfo = NULL,
    .sampleFormat = paFloat32,
    .suggestedLatency = Pa_GetDeviceInfo(device)->defaultHighOutputLatency
  };

  Audio * audio = g_malloc(sizeof(Audio));
  if (!audio) {
    return NULL;
  }

  audio->apu = apu;
  PaError err = Pa_OpenStream(&audio->stream,
                              NULL,
                              &output_parameters,
                              SAMPLE_RATE,
                              paFramesPerBufferUnspecified,
                              paNoFlag,
                              audio_callback,
                              audio);

  if (err != paNoError) {
    free(audio);
    return NULL;
  }

  return audio;
}

void audio_destroy(Audio * audio) {
  Pa_CloseStream(audio->stream);
  g_free(audio);
}

int audio_start(Audio * audio) {
  return Pa_StartStream(audio->stream) == paNoError;
}

int audio_stop(Audio * audio) {
  return Pa_StopStream(audio->stream) == paNoError;
}
