#ifndef NOISE_H
#define NOISE_H

#include <stdint.h>

typedef struct Noise Noise;

void noise_init(Noise * noise);
uint8_t noise_sample(Noise * noise);
void noise_period_tick(Noise * noise);
void noise_length_tick(Noise * noise);
void noise_envelope_tick(Noise * noise);

void noise_write(Noise * noise, uint8_t addr, uint8_t val);
uint8_t noise_read(Noise * noise, uint8_t addr);

#endif
