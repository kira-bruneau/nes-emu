#ifndef NOISE_H
#define NOISE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/APU_Noise
 */

typedef struct Noise Noise;
struct Noise {
  bool loop               : 1;
  bool envelope_disabled  : 1;
  uint8_t volume          : 4;
  bool mode               : 1;
  uint8_t period          : 4;
  uint8_t length          : 5;

  // Internal variables
  bool envelope_reload    : 1;
  uint8_t envelope_val    : 4;

  uint16_t shift_register : 15;

  uint16_t period_timer   : 12; // Must hold up to 4068
  uint8_t length_timer    : 5;
};

void noise_init(Noise * noise);
uint8_t noise_sample(Noise * noise);
void noise_period_tick(Noise * noise);
void noise_length_tick(Noise * noise);
void noise_envelope_tick(Noise * noise);

void noise_write(Noise * noise, uint8_t addr, uint8_t val);
uint8_t noise_read(Noise * noise, uint8_t addr);

#endif
