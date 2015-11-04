#include <stdlib.h>

#include "noise.h"

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/APU_Noise
 */

static uint16_t timer_periods[16] = {
  4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068
};

byte noise_sample(Noise * noise) {
  if (noise->length_counter_halt != 1 && noise->length_counter_val == 0) {
    return 0;
  }

  // TODO: This should use the original pseudo random algorithm
  return rand() % 16;
}

void noise_timer_tick(Noise * noise) {
  if (noise->timer_val == 0) {
    // TODO: Clock shift register
    noise->timer_val = timer_periods[noise->period];
  } else {
    noise->timer_val -= 1;
  }
}

void noise_length_counter_tick(Noise * noise) {
  if (noise->length_counter_halt != 1 && noise->length_counter_val != 0) {
    noise->length_counter_val -= 1;
  }
}

void noise_envelope_tick(Noise * noise) {
  (void)noise;
}

void noise_write(Noise * noise, byte addr, byte val) {
  (void)noise;
  (void)addr;
  (void)val;
}

byte noise_read(Noise * noise, byte addr) {
  (void)noise;
  (void)addr;
  return 0;
}
