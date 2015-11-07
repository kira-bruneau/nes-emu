#include <stdlib.h>

#include "noise.h"

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/APU_Noise
 */

struct Noise {
  bool loop                : 1;
  bool envelope_disabled   : 1;
  byte volume              : 4;
  bool mode                : 1;
  byte period              : 4;
  byte length              : 5;

  // Internal variables
  bool envelope_reload     : 1;
  byte envelope_val        : 4;

  uint16_t shift_register  : 15;

  uint16_t period_timer    : 12; // Must hold up to 4068
  byte length_timer        : 5;
};

static uint16_t noise_timer_periods[16] = {
  4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068
};

byte noise_sample(Noise * noise) {
  if (!noise->loop && noise->length_timer == 0) {
    return 0;
  }

  if (noise->shift_register & 1) {
    return 0;
  }

  if (noise->envelope_disabled) {
    return noise->volume;
  } else {
    return noise->envelope_val;
  }
}

static void noise_shift(Noise * noise) {
  byte other_bit = noise->mode ?
    (noise->shift_register >> 1 & 1) :
    (noise->shift_register >> 6 & 1);

  byte feedback = (noise->shift_register & 1) ^ other_bit;
  noise->shift_register >>= 1;
  noise->shift_register |= feedback << 14;
}

void noise_timer_tick(Noise * noise) {
  if (noise->period_timer == 0) {
    noise_shift(noise);
    noise->period_timer = noise_timer_periods[noise->period];
  } else {
    noise->period_timer -= 1;
  }
}

void noise_length_counter_tick(Noise * noise) {
  if (noise->loop != 1 && noise->length_timer != 0) {
    noise->length_timer -= 1;
  }
}

void noise_envelope_tick(Noise * noise) {
  if (noise->envelope_reload) {
    noise->envelope_val = 15;
    noise->envelope_reload = false;
  } else if (noise->envelope_val != 0) {
    noise->envelope_val--;
  } else if (noise->loop) {
    noise->envelope_val = 15;
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
