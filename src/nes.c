#include <stdio.h>

#include "nes.h"

void nes_init(NES * nes) {
  nes->cartridge = NULL;
  memory_init(&nes->mem, nes);
  cpu_init(&nes->cpu, nes);
  apu_init(&nes->apu, nes);
}

void nes_load(NES * nes, Cartridge * cartridge) {
  nes->cartridge = cartridge;
}
