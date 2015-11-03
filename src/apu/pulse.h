#ifndef PULSE_H
#define PULSE_H

#include "util.h"

typedef struct {
  byte duty                : 2;
  bool length_counter_halt : 1;
  bool constant_volume     : 1;
  byte envelope            : 4;

  bool sweep_unit          : 1;
  byte period              : 3;
  bool negate              : 1;
  byte shift               : 3;

  uint16_t timer           : 11;
  byte length_counter      : 5;

  // Internal variables
  uint16_t timer_val       : 11;
  byte length_counter_val  : 5;
} Pulse;

byte pulse_sample(Pulse * pulse);
void pulse_timer_tick(Pulse * pulse);
void pulse_length_counter_tick(Pulse * pulse);
void pulse_sweep_tick(Pulse * pulse);
void pulse_envelope_tick(Pulse * pulse);

void pulse_write(Pulse * pulse, byte addr, byte val);
byte pulse_read(Pulse * pulse, byte addr);

#endif
