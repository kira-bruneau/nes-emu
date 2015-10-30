#include <time.h>
#include <stdio.h>

#include "ui/audio.h"
#include "apu/triangle.h"

Triangle triangle = {
  .control_flag = true,
  .counter_reload = 0,
  .timer = 4,
  .length_counter_load = 0,
  .timer_val = 2047
};

static void tick(Audio * audio) {
  triangle_tick(&triangle);

  float output = (triangle_output(&triangle) - 8) / 8.0f;
  printf("%f\n", output);
  audio_write(audio, output);
}

static void render(unsigned int fps, Audio * audio) {
  if (fps > 1800000) {
    fps = 1800000;
  }


  for (;;) {
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int steps = 1/* 1800000 / fps */;
    while (steps-- > 0) {
      tick(audio);
    }

    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);

    struct timespec delta;
    delta.tv_sec = start.tv_sec - end.tv_sec;
    delta.tv_nsec = start.tv_nsec - end.tv_nsec;

    struct timespec sleep;
    sleep.tv_sec = 0;
    sleep.tv_nsec = 1000000000 / 50 /* 1000000000 / fps - delta.tv_nsec */;
    nanosleep(&sleep, NULL);
  }
}

int main(void) {
  Audio * audio = audio_create();
  audio_start(audio);
  render(60, audio);
  audio_stop(audio);
  audio_destroy(audio);
  return 0;
}
