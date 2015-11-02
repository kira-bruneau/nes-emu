#include <stdio.h>

#include "ui/render.h"

#include "ui/audio.h"
#include "apu/apu.h"

#define CPU_FREQUENCY 39375000 / 22.0f // ~ 1789773 Hz
#define FRAME_RATE 60

GLFWwindow * window;

APU apu;
Audio * audio;

double last_second;
int cycle_count;
int sample_count;
int render_clock;

void render_init(GLFWwindow * w) {
  window = w;

  apu_init(&apu);
  audio = audio_create();
  audio_start(audio);

  last_second = 0.0f;
  render_clock = 0;
}

static int frequency_scale(float ratio, int clock) {
  int int_part = (int)ratio;
  float frac_part = ratio - int_part;

  int result = int_part;
  if ((int)(clock * frac_part) != (int)((clock + 1) * frac_part)) {
    result += 1;
  }

  return result;
}

static void render_audio() {
  // Assume render loop is clocked at 60Hz (for now)
  double now = glfwGetTime();

  if (now - 1.0f >= last_second) {
    printf("%f (%i): %i, %i\n", now, render_clock, cycle_count, sample_count);
    cycle_count = sample_count = 0;
    last_second = now;
  }

  int num_cycles = frequency_scale(CPU_FREQUENCY / FRAME_RATE, render_clock);
  while (num_cycles-- != 0) {
    cycle_count += 1;
  }

  int num_samples = SAMPLE_RATE / FRAME_RATE;
  if (render_clock % (FRAME_RATE / 20) == 0) {
    ++num_samples;
  }

  while (num_samples-- != 0) {
    apu_tick(&apu);
    audio_write(audio, apu_sample(&apu));
    sample_count += 1;
  }

  render_clock += 1;
}

static void render_video() {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT);

  glLoadIdentity();

  glBegin(GL_QUADS);
  glVertex2f(-0.5f, -0.5f); // The bottom left corner
  glVertex2f(-0.5f, 0.5f); // The top left corner
  glVertex2f(0.5f, 0.5f); // The top right corner
  glVertex2f(0.5f, -0.5f); // The bottom right corner
  glEnd();
}

void render_loop() {
  render_audio();
  render_video();
}
