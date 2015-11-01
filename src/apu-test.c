#include <time.h>
#include <stdio.h>

#include "ui/audio.h"

#define MASTER_FREQUENCY 21477272 // Hz
#define CLOCK_DIVISOR 12
#define CPU_FREQUENCY MASTER_FREQUENCY / CLOCK_DIVISOR

static void render(unsigned int fps, APU * apu) {
  if (fps == 0) {
    fps = 1;
  } else if (fps > CPU_FREQUENCY) {
    fps = CPU_FREQUENCY;
  }

  for (;;) {
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int steps = CPU_FREQUENCY / 2 / fps;
    while (steps-- > 0) {
      apu_tick(apu);
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
  APU apu;
  apu_init(&apu);

  Audio * audio = audio_create(&apu);
  if (audio == NULL) {
    return 1;
  }

  audio_start(audio);

  for (;;) {
    struct timespec sleep;
    sleep.tv_sec = 1;
    sleep.tv_nsec = 0;
    nanosleep(&sleep, NULL);
  }

  audio_destroy(audio);
  return 0;
}
