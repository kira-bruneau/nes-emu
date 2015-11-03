#include <stdio.h>

#include "render.h"
#include "audio.h"

#include "apu/apu.h"
#include "clock.h"

#define FRAME_RATE 60

GLFWwindow * window;

Audio * audio;
APU apu;

int render_clock;

void render_init(GLFWwindow * w) {
  window = w;

  apu_init(&apu);

  // APU Tests
  apu.status.pulse1 = 1;
  apu.pulse1.length_counter_halt = 1;

  audio = audio_create(&apu);
  audio_start(audio);

  render_clock = 0;
}

void render_loop() {
  int cpu_cycles = frequency_scale(CPU_FREQUENCY / FRAME_RATE, render_clock);
  while (cpu_cycles-- != 0) {
    // Do cpu tick
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
