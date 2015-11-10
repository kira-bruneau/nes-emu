#ifndef PULSE_H
#define PULSE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/APU_Pulse
 */

typedef struct Pulse Pulse;
struct Pulse {
  uint8_t duty           : 2;
  bool loop              : 1;
  bool envelope_disabled : 1;
  uint8_t volume         : 4;

  bool sweep_enabled     : 1;
  uint8_t sweep_period   : 3;
  bool sweep_negate      : 1;
  uint8_t sweep_shift    : 3;

  uint16_t period        : 11;
  uint8_t length         : 5;

  // Internal variables
  bool channel           : 1;

  bool envelope_reload   : 1;
  uint8_t envelope_val   : 4;

  bool sweep_reload      : 1;
  uint8_t sweep_timer    : 3;

  uint16_t period_timer  : 11;
  uint8_t phase          : 3;

  uint8_t length_timer   : 8;
};

void pulse_init(Pulse * pulse, uint8_t channel);
uint8_t pulse_sample(Pulse * pulse);
void pulse_period_tick(Pulse * pulse);
void pulse_length_tick(Pulse * pulse);
void pulse_sweep_tick(Pulse * pulse);
void pulse_envelope_tick(Pulse * pulse);

void pulse_write(Pulse * pulse, uint8_t addr, uint8_t val);
uint8_t pulse_read(Pulse * pulse, uint8_t addr);

#endif
