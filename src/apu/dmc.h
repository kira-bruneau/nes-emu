#ifndef DMC_H
#define DMC_H

#include <stdint.h>
#include <stdbool.h>

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/APU_DMC
 */

typedef struct DMC DMC;
struct DMC {
  bool irq_enable        : 1;
  bool loop              : 1;
  uint8_t frequency      : 4;

  uint8_t loader_counter : 7;

  uint8_t sample_address : 8;
  uint8_t sample_length  : 8;
};

uint8_t dmc_sample(DMC * dmc);
void dmc_tick(DMC * dmc);
void dmc_write(DMC * dmc, uint8_t addr, uint8_t val);
uint8_t dmc_read(DMC * dmc, uint8_t addr);

#endif
