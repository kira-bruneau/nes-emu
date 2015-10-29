#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../util.h"

typedef struct {
  bool envelope_loop       : 1;
  byte linear_counter_load : 7;
  uint16_t timer           : 11;
  byte length_counter_load : 5;
} Triangle;

float triangle_output(Triangle * triangle);

#endif
