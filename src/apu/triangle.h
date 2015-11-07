#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "util.h"

typedef struct Triangle Triangle;

byte triangle_sample(Triangle * triangle);
void triangle_period_tick(Triangle * triangle);
void triangle_length_tick(Triangle * triangle);
void triangle_linear_tick(Triangle * triangle);

void triangle_write(Triangle * triangle, byte addr, byte val);
byte triangle_read(Triangle * triangle, byte addr);

#endif
