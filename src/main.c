#include <GLFW/glfw3.h>

#include <stdlib.h>

#include "ui/events.h"
#include "ui/render.h"

#define SCALE 4
#define WINDOW_WIDTH 256 * SCALE
#define WINDOW_HEIGHT 240 * SCALE

int main() {
  GLFWwindow * window;

  glfwSetErrorCallback(event_error);

  if (!glfwInit()) {
    return EXIT_FAILURE;
  }

  // Create window
  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "NES Emulator", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return EXIT_FAILURE;
  }

  // Register key events
  glfwSetKeyCallback(window, event_keypress);

  // Set OpenGL context to window
  glfwMakeContextCurrent(window);

  /* glfwSwapInterval(1); */

  render_init(window);

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    render_loop();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return EXIT_SUCCESS;
}
