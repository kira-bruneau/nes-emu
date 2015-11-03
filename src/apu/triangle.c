#include <stdlib.h>

#include "triangle.h"

void triangle_tick(Triangle * triangle) {
  triangle->timer_val -= 1;
}

byte triangle_sample(Triangle * triangle) {
  return abs((triangle->timer_val - 1024) / 64);
}
