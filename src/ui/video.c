#include <stdlib.h>

#include <GLFW/glfw3.h>
#include <glib.h>

#include "video.h"

struct Video {
  // PPU * ppu;
};

Video * video_create(void) {
  Video * video = g_malloc(sizeof(Video));
  return video;
}

void video_destroy(Video * video) {
  g_free(video);
}

void video_loop(Video * video) {
  (void)video;
  glLoadIdentity();

  glBegin(GL_QUADS);
  glVertex2f(-0.5f, -0.5f); // The bottom left corner
  glVertex2f(-0.5f, 0.5f); // The top left corner
  glVertex2f(0.5f, 0.5f); // The top right corner
  glVertex2f(0.5f, -0.5f); // The bottom right corner
  glEnd();
}
