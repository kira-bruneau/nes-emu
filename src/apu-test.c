#include <time.h>
#include <stdio.h>

#include "ui/audio.h"

#include "apu/triangle.h"
#include "apu/pulse.h"
#include "apu/noise.h"

#define CPU_FREQUENCY 1789773 // Hz

Pulse pulse = {
  .timer = 12,
  .timer_val = 0
};

Triangle triangle = {
  .timer = 15,
  .timer_val = 0
};

Noise noise = {

};

static void tick(Audio * audio) {
  triangle_tick(&triangle);
  pulse_tick(&pulse);
  noise_tick(&noise);

  byte output = triangle_output(&triangle);
  audio_write(audio, output / 7.5f - 1.0f);
}

static void render(unsigned int fps, Audio * audio) {
  if (fps == 0) {
    fps = 1;
  } else if (fps > CPU_FREQUENCY) {
    fps = CPU_FREQUENCY;
  }

  for (;;) {
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int steps = SAMPLE_RATE / fps;
    while (steps-- > 0) {
      tick(audio);
    }

    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);

    struct timespec delta;
    delta.tv_sec = end.tv_sec - start.tv_sec;
    delta.tv_nsec = end.tv_nsec - start.tv_nsec;

    struct timespec sleep;
    sleep.tv_sec = 0 - delta.tv_sec;
    sleep.tv_nsec = 1000000000 / fps - delta.tv_nsec - 100000;
    nanosleep(&sleep, NULL);
  }
}

int main(void) {
  Audio * audio = audio_create();
  audio_start(audio);
  render(30, audio);
  audio_stop(audio);
  audio_destroy(audio);
  return 0;
}
