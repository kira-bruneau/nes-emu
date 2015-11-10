#ifndef DMC_H
#define DMC_H

#include <stdint.h>

typedef struct DMC DMC;

uint8_t dmc_sample(DMC * dmc);
void dmc_tick(DMC * dmc);
void dmc_write(DMC * dmc, uint8_t addr, uint8_t val);
uint8_t dmc_read(DMC * dmc, uint8_t addr);

#endif
