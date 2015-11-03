#ifndef NOISE_H
#define NOISE_H

#include "util.h"

typedef struct {
  bool length_counter_halt : 1;
  bool constant_volume     : 1;
  byte envelope            : 4;
  bool loop_noise          : 1;
  byte noise_period        : 4;
  byte length_counter      : 5;

  // Internal variables
  byte length_counter_val  : 5;
} Noise;

byte noise_sample(Noise * noise);
void noise_tick(Noise * noise);
void noise_write(Noise * noise, byte addr, byte val);
byte noise_read(Noise * noise, byte addr);

#endif
