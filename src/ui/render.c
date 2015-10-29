#include "render.h"

GLFWwindow * window;

void render_init(GLFWwindow * w) {
  window = w;
}

void render_loop() {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT);

  glLoadIdentity();

  // I need to add orthogonal projection matrix

  glBegin(GL_QUADS);

  glVertex2f(-0.5f, -0.5f); // The bottom left corner
  glVertex2f(-0.5f, 0.5f); // The top left corner
  glVertex2f(0.5f, 0.5f); // The top right corner
  glVertex2f(0.5f, -0.5f); // The bottom right corner

  glEnd();

  /* double time = glfwGetTime(); */
}
