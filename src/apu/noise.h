#ifndef NOISE_H
#define NOISE_H

#include "util.h"

typedef struct Noise Noise;

void noise_init(Noise * noise);
byte noise_sample(Noise * noise);
void noise_period_tick(Noise * noise);
void noise_length_tick(Noise * noise);
void noise_envelope_tick(Noise * noise);

void noise_write(Noise * noise, byte addr, byte val);
byte noise_read(Noise * noise, byte addr);

#endif
