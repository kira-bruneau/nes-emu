#include <stdio.h>

#include <GLFW/glfw3.h>

#include "apu/apu.h"
#include "ui/audio.h"

#define CPU_FREQUENCY 1789773

int main(void) {
  GLFWwindow * window;

  if (!glfwInit()) {
    return 1;
  }

  // Create window
  window = glfwCreateWindow(500, 500, "NES Emulator", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return 1;
  }

  // Set OpenGL context to window
  glfwMakeContextCurrent(window);

  APU apu;
  apu_init(&apu);

  Audio * audio = audio_create();
  audio_start(audio);

  // Render loop
  double timestamp = 0;
  while (!glfwWindowShouldClose(window)) {
    double now = glfwGetTime();
    double delta = now - timestamp;
    timestamp = now;

    int steps = delta * SAMPLE_RATE;
    while (steps-- > 0) {
      apu_tick(&apu);
      audio_write(audio, apu_sample(&apu));
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  audio_destroy(audio);
  return 0;
}
