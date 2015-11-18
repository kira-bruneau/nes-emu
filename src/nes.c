#include <stdio.h>

#include "nes.h"

void nes_init(NES * nes) {
  nes->cartridge = NULL;
  memory_init(&nes->mem);
  cpu_init(&nes->cpu);
  apu_init(&nes->apu);
}

void nes_load(NES * nes, Cartridge * cartridge) {
  nes->cartridge = cartridge;
  memory_reset(&nes->mem);
  cpu_reset(&nes->cpu);
  apu_reset(&nes->apu);
  // TODO: DEBUG
  cpu_debug(&nes->cpu);
}
