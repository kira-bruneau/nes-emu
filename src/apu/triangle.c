#include <stdlib.h>

#include "triangle.h"

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/APU_Triangle
 */

byte triangle_sample(Triangle * triangle) {
  if (triangle->control_flag != 0 && triangle->length_counter_val == 0) {
    return 0;
  }

  // TODO: This should use the original pesudo triangle waveform
  // 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
  // 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
  return abs((triangle->timer_val - 1024) / 64);
}

void triangle_tick(Triangle * triangle) {
  // Note: triangle timer ticks at same frequency as the CPU (2*APU)
  triangle->timer_val -= 2;

  if (triangle->control_flag != 0 && triangle->length_counter_val != 0) {
    triangle->length_counter_val -= 1;
  }
}

void triangle_write(Triangle * triangle, byte addr, byte val) {
  (void)triangle;
  (void)addr;
  (void)val;
}

byte triangle_read(Triangle * triangle, byte addr) {
  (void)triangle;
  (void)addr;
  return 0;
}
