#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>

typedef struct Triangle Triangle;

uint8_t triangle_sample(Triangle * triangle);
void triangle_period_tick(Triangle * triangle);
void triangle_length_tick(Triangle * triangle);
void triangle_linear_tick(Triangle * triangle);

void triangle_write(Triangle * triangle, uint8_t addr, uint8_t val);
uint8_t triangle_read(Triangle * triangle, uint8_t addr);

#endif
