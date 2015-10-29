#include <stdlib.h>
#include <stdio.h>
#include <portaudio.h>

#include "apu/triangle.h"

#define SAMPLE_RATE 44100

static int audio_callback(const void * input_buffer,
                          void * output_buffer,
                          unsigned long frames_per_buffer,
                          const PaStreamCallbackTimeInfo * time_info,
                          PaStreamCallbackFlags status_flags,
                          void * user_data) {
  (void)input_buffer;
  (void)time_info;
  (void)status_flags;

  Triangle * triangle = (Triangle *)user_data;
  float * out = (float *)output_buffer;

  unsigned int i;
  for (i = 0; i < frames_per_buffer; i++) {
    *out++ = triangle_output(triangle);
  }

  return 0;
}

int main(void) {
  PaError err = Pa_Initialize();
  if (err != paNoError) goto error;

  Triangle triangle;

  PaStream * stream;
  err = Pa_OpenDefaultStream(&stream,
                             0, // input channels (none)
                             1, // output channels (mono)
                             paFloat32,
                             SAMPLE_RATE,
                             paFramesPerBufferUnspecified,
                             audio_callback,
                             &triangle);

  if (err != paNoError) goto error;

  err = Pa_StartStream(stream);
  if (err != paNoError) goto error;

  Pa_Sleep(100000);

  err = Pa_StopStream(stream);
  if (err != paNoError) goto error;

  err = Pa_CloseStream(stream);
  if (err != paNoError) goto error;

  err = Pa_Terminate();
  if (err != paNoError) goto error;

  return 0;

 error:
  fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
  return 1;
}
