#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <portaudio.h>

#include "audio.h"
#include "vendor/pa_ringbuffer.h"

#define BUFFER_SIZE 65536

struct Audio {
  PaStream * stream;
  PaUtilRingBuffer buffer;
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

  size_t num_read = PaUtil_ReadRingBuffer(&audio->buffer, out, frames_per_buffer);
  memset(out + num_read, 0, sizeof(float) * (frames_per_buffer - num_read));
  return 0;
}

Audio * audio_create() {
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
        .suggestedLatency = device_info->defaultLowOutputLatency
      };

      Audio * audio = malloc(sizeof(Audio) + sizeof(float) * BUFFER_SIZE);
      void * data = audio + 1;

      if (audio != NULL) {
        int res = PaUtil_InitializeRingBuffer(&audio->buffer, sizeof(float), BUFFER_SIZE, data);
        printf("%i\n", res);

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

size_t audio_write(Audio * audio, float val) {
  return PaUtil_WriteRingBuffer(&audio->buffer, &val, 1);
}
