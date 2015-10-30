#include <stdlib.h>

#include "triangle.h"

void triangle_tick(Triangle * triangle) {
  triangle->timer_val -= triangle->timer;
}

byte triangle_output(Triangle * triangle) {
  return abs((triangle->timer_val - 1024) / 64);
}
