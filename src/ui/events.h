#ifndef KEYMAP_H
#define KEYMAP_H

#include <GLFW/glfw3.h>

void event_keypress(GLFWwindow * window, int key, int scancode, int action, int mods);
void event_error(int error, const char * description);

#endif
