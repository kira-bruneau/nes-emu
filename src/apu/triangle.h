#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../util.h"

typedef struct {
  bool control_flag        : 1;
  byte counter_reload      : 7;
  uint16_t timer           : 11;
  byte length_counter_load : 5;

  uint16_t timer_val     : 11;
} Triangle;

void triangle_tick(Triangle * triangle);
byte triangle_sample(Triangle * triangle);

#endif
