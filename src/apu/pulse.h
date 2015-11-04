#ifndef PULSE_H
#define PULSE_H

#include "util.h"

typedef struct Pulse Pulse;

byte pulse_sample(Pulse * pulse);
void pulse_timer_tick(Pulse * pulse);
void pulse_length_counter_tick(Pulse * pulse);
void pulse_sweep_tick(Pulse * pulse);
void pulse_envelope_tick(Pulse * pulse);

void pulse_write(Pulse * pulse, byte addr, byte val);
byte pulse_read(Pulse * pulse, byte addr);

#endif
