#include "triangle.h"
#include "length_table.h"

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/APU_Triangle
 */

struct Triangle {
  bool control_flag     : 1;
  byte counter_reload   : 7;
  uint16_t period       : 11;
  byte length           : 5;

  // Internal variables
  uint16_t period_timer : 11;
  byte period_val       : 5;
  byte length_timer     : 5;
  bool linear_reload    : 1;
  byte linear_timer     : 7;
};

static byte triangle_sequencer[32] = {
  15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

void triangle_init(Triangle * triangle) {
  triangle->control_flag = 1;
  triangle->counter_reload = 15;
  triangle->period = 500;
  triangle->length = 1;

  // Internal variables
  triangle->period_timer = triangle->period;
  triangle->period_val = 0;
  triangle->length_timer = length_table[triangle->length];
  triangle->linear_reload = 0;
  triangle->linear_timer = 0;
}

byte triangle_sample(Triangle * triangle) {
  // TODO: Liner counter gate

  if (triangle->control_flag && triangle->length_timer == 0) {
    return 0;
  }

  return triangle_sequencer[triangle->period_val];
}

void triangle_period_tick(Triangle * triangle) {
  if (triangle->period_timer == 0) {
    triangle->period_val += 1;
    triangle->period_timer = triangle->period;
  } else {
    triangle->period_timer -= 1;
  }
}

void triangle_length_tick(Triangle * triangle) {
  if (triangle->control_flag != 0 && triangle->length_timer != 0) {
    triangle->length_timer -= 1;
  }
}

void triangle_linear_tick(Triangle * triangle) {
  if (triangle->linear_reload) {
    triangle->linear_timer = triangle->counter_reload;
  } else if (triangle->linear_timer != 0) {
    triangle->linear_timer--;
  }

  if (!triangle->control_flag) {
    triangle->linear_reload = false;
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
