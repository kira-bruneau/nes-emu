#include <stdio.h>

#include "ui/render.h"

#include "ui/audio.h"
#include "apu/apu.h"

#define CPU_FREQUENCY 39375000 / 22.0f // ~ 1789773 Hz
#define FRAME_RATE 60

GLFWwindow * window;

Audio * audio;
APU apu;

int render_clock;
int cycle_count;
double last_second;

void render_init(GLFWwindow * w) {
  window = w;

  apu_init(&apu);
  audio = audio_create(&apu);
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

void render_loop() {
  double now = glfwGetTime();

  if (now - 1.0f >= last_second) {
    printf("%f (%i): %i, %i\n", now, render_clock, cycle_count);
    cycle_count = 0;
    last_second = now;
  }

  int cpu_cycles = frequency_scale(CPU_FREQUENCY / FRAME_RATE, render_clock);
  while (cpu_cycles-- != 0) {
    cycle_count += 1;
  }

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

  render_clock += 1;
}
