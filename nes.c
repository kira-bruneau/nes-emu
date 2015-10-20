#include <stdio.h>

#include "nes.h"
#include "cartridge.h"
#include "memory.h"

NES * nes_new(GFile * rom_file) {
  NES * nes = g_malloc(sizeof(NES));

  Cartridge * cartridge = cartridge_new(rom_file);
  Memory * memory = memory_new(cartridge, 2048);
  nes->cpu = cpu_new(memory);

  char buffer[256];
  while(fgets(buffer, ARRAY_LENGTH(buffer), stdin) != NULL) {
    cpu_next_instr(nes->cpu);
  }
  return nes;
}
