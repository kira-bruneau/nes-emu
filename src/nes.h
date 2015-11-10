#ifndef NES_H
#define NES_H

#include "cartridge/cartridge.h"
#include "memory/memory.h"
#include "cpu/cpu.h"
#include "apu/apu.h"

typedef struct NES NES;
struct NES {
  Cartridge * cartridge;
  Memory * mem;
  CPU * cpu;
  APU * apu;
};

NES * nes_create(void);
void nes_destroy(NES * nes);

void nes_load(NES * nes, Cartridge * cartridge);

#endif
