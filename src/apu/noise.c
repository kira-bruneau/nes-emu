#include "noise.h"
#include "length_table.h"

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

void noise_init(Noise * noise) {
  noise->shift_register = 1;
}

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

void noise_period_tick(Noise * noise) {
  if (noise->period_timer == 0) {
    noise_shift(noise);
    noise->period_timer = noise_timer_periods[noise->period];
  } else {
    noise->period_timer--;
  }
}

void noise_length_tick(Noise * noise) {
  if (!noise->loop && noise->length_timer != 0) {
    noise->length_timer--;
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
  switch (addr) {
  case 0:
    noise->loop = (val >> 5) & 1;
    noise->envelope_disabled = (val >> 4) & 1;
    noise->volume = (val >> 0) & 15;
    break;
  case 2:
    noise->mode = (val >> 7) & 1;
    noise->period = (val >> 0) & 15;
    break;
  case 3:
    noise->length = (val >> 3) & 31;
    noise->length_timer = length_table[noise->length];
    break;
  }
}

byte noise_read(Noise * noise, byte addr) {
  byte val = 0;

  switch (addr) {
  case 0:
    val |= (noise->loop & 1) << 5;
    val |= (noise->envelope_disabled & 1) << 4;
    if (noise->envelope_disabled) {
      val |= (noise->volume & 15) << 0;
    } else {
      val |= (noise->envelope_val & 15) << 0;
    }
    break;
  case 2:
    val |= (noise->mode & 1) << 7;
    val |= (noise->period & 15) << 0;
    break;
  case 3:
    val |= (noise->length & 31) << 3;
    break;
  }

  return val;
}
