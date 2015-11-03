#include <stdlib.h>

#include "noise.h"

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/APU_Noise
 */

byte noise_sample(Noise * noise) {
  if (noise->length_counter_halt != 1 && noise->length_counter_val == 0) {
    return 0;
  }

  // TODO: This should use the original pseudo random algorithm
  return rand() % 16;
}

void noise_tick(Noise * noise) {
  if (noise->length_counter_halt != 1 && noise->length_counter_val != 0) {
    noise->length_counter_val -= 1;
  }
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
