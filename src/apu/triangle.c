#include <stdlib.h>

#include "triangle.h"

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/APU_Triangle
 */

static byte sequencer[32] = {
  15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

byte triangle_sample(Triangle * triangle) {
  // TODO: Liner counter gate

  if (triangle->control_flag != 0 && triangle->length_counter_val == 0) {
    return 0;
  }

  return sequencer[triangle->sequence_val];
}

void triangle_timer_tick(Triangle * triangle) {
  if (triangle->timer_val == 0) {
    triangle->sequence_val += 1;
    triangle->timer_val = triangle->timer;
  } else {
    triangle->timer_val -= 1;
  }
}

void triangle_length_counter_tick(Triangle * triangle) {
  if (triangle->control_flag != 0 && triangle->length_counter_val != 0) {
    triangle->length_counter_val -= 1;
  }
}

void triangle_linear_counter_tick(Triangle * triangle) {
  (void)triangle;
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
