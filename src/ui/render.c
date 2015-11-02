#include <stdio.h>

#include "ui/render.h"

#include "ui/audio.h"
#include "apu/apu.h"

#define CPU_FREQUENCY 1789773

GLFWwindow * window;

APU apu;
Audio * audio;

double last_second;
int cycle_count;
int sample_count;

void render_init(GLFWwindow * w) {
  window = w;

  apu_init(&apu);
  audio = audio_create();
  audio_start(audio);

  last_second = 0.0f;
}

static void render_audio() {
  double now = glfwGetTime();

  if (now - 1.0f >= last_second) {
    printf("%f: %i, %i\n", now, cycle_count, sample_count);
    cycle_count = sample_count = 0;
    last_second = now;
  }

  int cpu_steps = 29830; /* 29829.54545454545454545455 */
  while (cpu_steps-- > 0) {
    cycle_count += 1;
  }

  int sample_steps = 735;
  while (sample_steps-- > 0) {
    apu_tick(&apu);
    audio_write(audio, apu_sample(&apu));
    sample_count += 1;
  }
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
