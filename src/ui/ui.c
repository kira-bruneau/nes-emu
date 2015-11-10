#include <stdlib.h>

#include <GLFW/glfw3.h>
#include <glib.h>

#include "ui.h"
#include "events.h"
#include "clock.h"

#define SCALE 4
#define WINDOW_WIDTH 256 * SCALE
#define WINDOW_HEIGHT 240 * SCALE
#define FRAME_RATE 60

void ui_init(UI * ui) {
  nes_init(&ui->nes);
  video_init(&ui->video);
  ui->audio = audio_create(&ui->nes.apu);
}

void ui_deinit(UI * ui) {
  if (ui->audio) {
    audio_destroy(ui->audio);
  }
}

int ui_run(UI * ui, Cartridge * cartridge) {
  nes_load(&ui->nes, cartridge);

  GLFWwindow * window = glfwCreateWindow(
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    "NES Emulator",
    NULL, NULL
  );

  if (!window) {
    return 0;
  }

  glfwSetKeyCallback(window, event_keypress);
  glfwMakeContextCurrent(window);

  if (ui->audio) {
    audio_start(ui->audio);
  }

  int render_clock = 0;
  int apu_timer = 0;
  
  while (!glfwWindowShouldClose(window)) {
    int cpu_cycles = frequency_scale(CPU_FREQUENCY / FRAME_RATE, render_clock);
    while (cpu_cycles-- != 0) {
      cpu_next_instr(&ui->nes.cpu);
      
      int ppu_cycles = 3;
      while (ppu_cycles-- != 0) {
        /* ppu_tick(&ui->nes.ppu); */
      }

      if (apu_timer != 0) {
        apu_timer--;
      } else {
        apu_timer = 1;
        apu_tick(&ui->nes.apu);
      }
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    video_render(&ui->video);

    glfwSwapBuffers(window);
    glfwPollEvents();
    render_clock++;
  }

  if (ui->audio) {
    audio_stop(ui->audio);
  }
  
  glfwDestroyWindow(window);
  return 1;
}
