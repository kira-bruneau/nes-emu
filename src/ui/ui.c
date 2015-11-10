#include <stdlib.h>

#include <GLFW/glfw3.h>
#include <glib.h>

#include "ui.h"
#include "video.h"
#include "audio.h"
#include "events.h"
#include "clock.h"

#define SCALE 4
#define WINDOW_WIDTH 256 * SCALE
#define WINDOW_HEIGHT 240 * SCALE
#define FRAME_RATE 60

struct UI {
  NES * nes;
  Video * video;
  Audio * audio;
};

int ui_init(void) {
  if (glfwInit() == GL_FALSE) {
    return 0;
  }
  
  if (!audio_init()) {
    return 0;
  }
  
  return 1;
}

void ui_terminate(void) {
  glfwTerminate();
  audio_terminate();
}

UI * ui_create(NES * nes) {
  UI * ui = g_malloc(sizeof(UI));
  ui->nes = nes;
  ui->video = video_create();
  ui->audio = audio_create(&nes->apu);
  return ui;
}

void ui_destroy(UI * ui) {
  video_destroy(ui->video);
  audio_destroy(ui->audio);
  g_free(ui);
}

int ui_run(UI * ui) {
  GLFWwindow * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "NES Emulator", NULL, NULL);
  if (!window) {
    return 0;
  }

  glfwSetKeyCallback(window, event_keypress);
  glfwMakeContextCurrent(window);
  audio_start(ui->audio);

  int render_clock = 0;
  while (!glfwWindowShouldClose(window)) {
    int cpu_cycles = frequency_scale(CPU_FREQUENCY / FRAME_RATE, render_clock);
    while (cpu_cycles-- != 0) {
      cpu_next_instr(&ui->nes->cpu);
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    video_loop(ui->video);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    
    render_clock++;
  }

  glfwDestroyWindow(window);
  return 1;
}
