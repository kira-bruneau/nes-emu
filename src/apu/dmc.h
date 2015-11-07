#ifndef DMC_H
#define DMC_H

#include "util.h"

typedef struct DMC DMC;

byte dmc_sample(DMC * dmc);
void dmc_tick(DMC * dmc);
void dmc_write(DMC * dmc, byte addr, byte val);
byte dmc_read(DMC * dmc, byte addr);

#endif
