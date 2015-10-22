#include <stdio.h>

#include "nes.h"
#include "cartridge.h"
#include "memory.h"

NES * nes_new(GFile * rom_file) {
  NES * nes = g_malloc(sizeof(NES));

  Cartridge * cartridge = cartridge_new(rom_file);
  Memory * memory = memory_new(cartridge);
  nes->cpu = cpu_new(memory);

  cpu_test_interactive(nes->cpu);
  return nes;
}
