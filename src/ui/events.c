#include <stdio.h>

#include "events.h"
#include "array.h"

typedef enum Event {
  EVENT_NONE,
  EVENT_EXIT,
  EVENT_DPAD_UP,
  EVENT_DPAD_DOWN,
  EVENT_DPAD_LEFT,
  EVENT_DPAD_RIGHT,
  EVENT_SELECT,
  EVENT_START,
  EVENT_A,
  EVENT_B,
  NUM_EVENTS
} Event;

static Event key_to_event(int key) {
  static struct {
    Event event;
    int key;
  } event_key_map[] = {
    {EVENT_EXIT, GLFW_KEY_ESCAPE},
    {EVENT_DPAD_UP, GLFW_KEY_E},
    {EVENT_DPAD_DOWN, GLFW_KEY_D},
    {EVENT_DPAD_LEFT, GLFW_KEY_S},
    {EVENT_DPAD_RIGHT, GLFW_KEY_F},
    {EVENT_SELECT, GLFW_KEY_G},
    {EVENT_START, GLFW_KEY_H},
    {EVENT_A, GLFW_KEY_K},
    {EVENT_B, GLFW_KEY_J}
  };

  size_t i;
  for (i = 0; i < ARRAY_LENGTH(event_key_map); ++i) {
    if (event_key_map[i].key == key) {
      return event_key_map[i].event;
    }
  }

  return EVENT_NONE;
}

void event_keypress(GLFWwindow * window, int key, int scancode, int action, int mods) {
  (void)scancode;
  (void)mods;

  if (action != GLFW_PRESS) {
    return;
  }

  switch (key_to_event(key)) {
  case EVENT_EXIT:
    glfwSetWindowShouldClose(window, GL_TRUE);
    break;
  case EVENT_DPAD_UP:
    puts("DPAD_UP");
    break;
  case EVENT_DPAD_DOWN:
    puts("DPAD_DOWN");
    break;
  case EVENT_DPAD_LEFT:
    puts("DPAD_LEFT");
    break;
  case EVENT_DPAD_RIGHT:
    puts("DPAD_RIGHT");
    break;
  case EVENT_SELECT:
    puts("SELECT");
    break;
  case EVENT_START:
    puts("START");
    break;
  case EVENT_A:
    puts("A");
    break;
  case EVENT_B:
    puts("B");
    break;
  default:
    break;
  }
}

void event_error(int error, const char * description) {
  (void)error;
  fputs(description, stderr);
}
