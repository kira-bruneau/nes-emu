#ifndef DMC_H
#define DMC_H

#include "util.h"

typedef struct {
  bool irq_enable          : 1;
  bool loop                : 1;
  byte frequency           : 4;

  byte loader_counter      : 7;

  byte sample_address      : 8;
  byte sample_length       : 8;
} DMC;

byte dmc_sample(DMC * dmc);
void dmc_tick(DMC * dmc);
void dmc_write(DMC * dmc, byte addr, byte val);
byte dmc_read(DMC * dmc, byte addr);

#endif
