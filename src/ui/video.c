#include <GLFW/glfw3.h>

#include "video.h"

void video_init(Video * video) {
  (void)video;
}

void video_render(Video * video) {
  (void)video;

  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  glBegin(GL_QUADS);
  glVertex2f(-0.5f, -0.5f);
  glVertex2f(-0.5f, 0.5f);
  glVertex2f(0.5f, 0.5f);
  glVertex2f(0.5f, -0.5f);
  glEnd();
}
