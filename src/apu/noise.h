#ifndef NOISE_H
#define NOISE_H

#include "../util.h"

typedef struct {
  bool envelope_loop       : 1;
  bool constant_volume     : 1;
  byte volume              : 4;
  bool loop_noise          : 1;
  byte noise_period        : 4;
  byte length_counter_load : 5;
} Noise;

void noise_tick(Noise * noise);
byte noise_output(Noise * noise);

#endif
