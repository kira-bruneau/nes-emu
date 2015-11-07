#ifndef PULSE_H
#define PULSE_H

#include "util.h"

typedef struct Pulse Pulse;

void pulse_init(Pulse * pulse, byte channel);
byte pulse_sample(Pulse * pulse);
void pulse_period_tick(Pulse * pulse);
void pulse_length_tick(Pulse * pulse);
void pulse_sweep_tick(Pulse * pulse);
void pulse_envelope_tick(Pulse * pulse);

void pulse_write(Pulse * pulse, byte addr, byte val);
byte pulse_read(Pulse * pulse, byte addr);

#endif
