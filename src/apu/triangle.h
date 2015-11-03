#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "util.h"

typedef struct {
  bool control_flag       : 1;
  byte counter_reload     : 7;
  uint16_t timer          : 11;
  byte length_counter     : 5;

  // Internal variables
  uint16_t timer_val      : 11;
  uint16_t sequence_val   : 5;
  byte length_counter_val : 5;
  byte linear_counter_val : 5;
} Triangle;

byte triangle_sample(Triangle * triangle);
void triangle_timer_tick(Triangle * triangle);
void triangle_length_counter_tick(Triangle * triangle);
void triangle_linear_counter_tick(Triangle * triangle);

void triangle_write(Triangle * triangle, byte addr, byte val);
byte triangle_read(Triangle * triangle, byte addr);

#endif
