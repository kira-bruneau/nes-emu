#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <portaudio.h>

#include "audio.h"
#include "clock.h"

#define SAMPLE_RATE 44100

struct Audio {
  PaStream * stream;
  APU * apu;
  int sample_clock;
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

  unsigned long i = 0;
  for (i = 0; i < frames_per_buffer; ++i) {
    int apu_cycles = frequency_scale(APU_FREQUENCY / SAMPLE_RATE, audio->sample_clock);
    while (apu_cycles-- != 0) {
      apu_tick(audio->apu);
    }

    out[i] = apu_sample(audio->apu);
    audio->sample_clock += 1;
  }

  return 0;
}

Audio * audio_create(APU * apu) {
  PaError err;

  err = Pa_Initialize();
  if (err == paNoError) {
    PaDeviceIndex device = Pa_GetDefaultOutputDevice();
    if (device != paNoDevice) {
      const PaDeviceInfo * device_info = Pa_GetDeviceInfo(device);

      PaStreamParameters output_parameters = {
        .channelCount = 1,
        .device = device,
        .hostApiSpecificStreamInfo = NULL,
        .sampleFormat = paFloat32,
        .suggestedLatency = device_info->defaultHighOutputLatency
      };

      Audio * audio = malloc(sizeof(Audio));
      if (audio != NULL) {
        audio->apu = apu;

        err = Pa_OpenStream(&audio->stream,
                            NULL,
                            &output_parameters,
                            SAMPLE_RATE,
                            paFramesPerBufferUnspecified,
                            paNoFlag,
                            audio_callback,
                            audio);

        if (err == paNoError) {
          return audio;
        }

        free(audio);
      }
    }

    Pa_Terminate();
  }

  return NULL;
}

void audio_destroy(Audio * audio) {
  Pa_CloseStream(audio->stream);
  Pa_Terminate();
  free(audio);
}

int audio_start(Audio * audio) {
  return Pa_StartStream(audio->stream) == paNoError;
}

int audio_stop(Audio * audio) {
  return Pa_StopStream(audio->stream) == paNoError;
}
