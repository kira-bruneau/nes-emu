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
  byte phase            : 5;
  byte length_timer     : 5;
  bool linear_reload    : 1;
  byte linear_timer     : 7;
};

static byte triangle_sequencer[32] = {
  15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

void triangle_init(Triangle * triangle) {
  (void)triangle;
}

byte triangle_sample(Triangle * triangle) {
  // TODO: Liner counter gate

  if (triangle->control_flag && triangle->length_timer == 0) {
    return 0;
  }

  return triangle_sequencer[triangle->phase];
}

void triangle_period_tick(Triangle * triangle) {
  if (triangle->period_timer == 0) {
    triangle->phase++;
    triangle->period_timer = triangle->period;
  } else {
    triangle->period_timer--;
  }
}

void triangle_length_tick(Triangle * triangle) {
  if (triangle->control_flag != 0 && triangle->length_timer != 0) {
    triangle->length_timer--;
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
  switch (addr) {
  case 0:
    triangle->control_flag = (val >> 7) & 1;
    triangle->counter_reload = (val >> 0) & 127;
    triangle->linear_reload = true;
    break;
  case 2:
    triangle->period = (triangle->period & 0xF0) | val;
    break;
  case 3:
    triangle->length = (val >> 3) & 31;
    triangle->period = (val & 0x07) << 8 | (triangle->period & 0x0F);
    triangle->length_timer = length_table[triangle->length];
    triangle->phase = 0;
    break;
  }
}

byte triangle_read(Triangle * triangle, byte addr) {
  byte val = 0;

  switch (addr) {
  case 0:
    val |= (triangle->control_flag & 1) << 7;
    val |= (triangle->counter_reload & 127) << 0;
    break;
  case 2:
    val |= triangle->period & 0x0F;
    break;
  case 3:
    val |= (triangle->length & 31) << 3;
    val |= (triangle->period & 0x70) >> 8;
    break;
  }

  return 0;
}
