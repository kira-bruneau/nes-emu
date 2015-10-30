#include "triangle.h"

#include <stdio.h>
void triangle_tick(Triangle * triangle) {
  triangle->timer_val -= 1;
}

byte triangle_output(Triangle * triangle) {
  return triangle->timer_val / 128;
}
