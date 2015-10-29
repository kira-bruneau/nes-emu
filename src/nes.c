#include <stdio.h>

#include "nes.h"
#include "memory/memory.h"
#include "cpu/cpu.h"

struct NES {
  Memory * mem;
  CPU * cpu;
};

NES * nes_new(void) {
  NES * nes = g_malloc(sizeof(NES));
  nes->mem = memory_new();
  nes->cpu = cpu_new(nes->mem);
  return nes;
}

void nes_load(NES * nes, Cartridge * cartridge) {
  memory_map_cartridge(nes->mem, cartridge);
  cpu_debug(nes->cpu);
}
