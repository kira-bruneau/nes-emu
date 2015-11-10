#ifndef PULSE_H
#define PULSE_H

#include <stdint.h>

typedef struct Pulse Pulse;

void pulse_init(Pulse * pulse, uint8_t channel);
uint8_t pulse_sample(Pulse * pulse);
void pulse_period_tick(Pulse * pulse);
void pulse_length_tick(Pulse * pulse);
void pulse_sweep_tick(Pulse * pulse);
void pulse_envelope_tick(Pulse * pulse);

void pulse_write(Pulse * pulse, uint8_t addr, uint8_t val);
uint8_t pulse_read(Pulse * pulse, uint8_t addr);

#endif
