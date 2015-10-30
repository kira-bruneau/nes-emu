#ifndef PULSE_H
#define PULSE_H

#include "../util.h"

typedef struct {
  byte duty                : 2;
  bool envelope_loop       : 1;
  bool constant_volume     : 1;
  byte volume              : 4;

  bool sweep_unit          : 1;
  byte period              : 3;
  bool negate              : 1;
  byte shift               : 3;

  uint16_t timer           : 11;
  byte length_counter_load : 5;

  uint16_t timer_val       : 11;
} Pulse;

void pulse_tick(Pulse * pulse);
byte pulse_output(Pulse * pulse);

#endif
