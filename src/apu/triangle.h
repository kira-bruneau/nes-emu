#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/APU_Triangle
 */

typedef struct Triangle Triangle;
struct Triangle {
  bool control_flag      : 1;
  uint8_t counter_reload : 7;
  uint16_t period        : 11;
  uint8_t length         : 5;

  // Internal variables
  uint16_t period_timer  : 11;
  uint8_t phase          : 5;
  uint8_t length_timer   : 5;
  bool linear_reload     : 1;
  uint8_t linear_timer   : 7;
};

void triangle_init(Triangle * pulse);
uint8_t triangle_sample(Triangle * triangle);
void triangle_period_tick(Triangle * triangle);
void triangle_length_tick(Triangle * triangle);
void triangle_linear_tick(Triangle * triangle);

void triangle_write(Triangle * triangle, uint8_t addr, uint8_t val);
uint8_t triangle_read(Triangle * triangle, uint8_t addr);

#endif
