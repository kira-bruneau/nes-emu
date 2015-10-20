#include "nes.h"

#include "cartridge.h"
#include "memory.h"

NES * nes_new(GFile * rom_file) {
  NES * nes = g_malloc(sizeof(NES));
  nes->cpu = cpu_new(memory_new(cartridge_new(rom_file), 2048));

  int i;
  for (i = 0; i < 1; ++i) {
    cpu_eval_next(nes->cpu);
  }
  return nes;
}
