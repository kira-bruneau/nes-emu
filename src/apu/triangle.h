#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "util.h"

typedef struct Triangle Triangle;

byte triangle_sample(Triangle * triangle);
void triangle_timer_tick(Triangle * triangle);
void triangle_length_counter_tick(Triangle * triangle);
void triangle_linear_counter_tick(Triangle * triangle);

void triangle_write(Triangle * triangle, byte addr, byte val);
byte triangle_read(Triangle * triangle, byte addr);

#endif
