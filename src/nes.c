#include <stdio.h>

#include "nes.h"

NES * nes_create(void) {
  NES * nes = g_malloc(sizeof(NES));
  nes->cartridge = NULL;
  nes->mem = memory_create(nes);
  nes->cpu = cpu_create(nes);
  nes->apu = apu_create(nes);
  return nes;
}

void nes_load(NES * nes, Cartridge * cartridge) {
  nes->cartridge = cartridge;
  cpu_debug(nes->cpu);
}
