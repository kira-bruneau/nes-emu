#include <string.h>

#include <glib.h>

#include "nes.h"
#include "memory.h"

struct Memory {
  NES * nes;
  byte * ram;
};

Memory * memory_create(NES * nes) {
  Memory * mem = g_malloc(sizeof(Memory));
  mem->nes = nes;
  mem->ram = g_malloc0(MEMORY_RAM_SIZE);
  return mem;
}

void memory_destroy(Memory * mem) {
  g_free(mem->ram);
  g_free(mem);
}

void memory_reset(Memory * mem) {
  memset(mem->ram, 0, MEMORY_RAM_SIZE);
}

byte memory_read(Memory * mem, uint16_t addr) {
  if (addr < MEMORY_RAM_END) {
    return mem->ram[addr % MEMORY_RAM_SIZE];

  } else if (addr >= MEMORY_WAVEFORMS && addr < MEMORY_WAVEFORMS_END) {
    return apu_read(mem->nes->apu, addr - MEMORY_WAVEFORMS);

  } else if (addr == MEMORY_APU_STATUS) {
    return apu_read(mem->nes->apu, APU_STATUS);

  } else if (addr == MEMORY_APU_FRAME_COUNTER) {
    return apu_read(mem->nes->apu, APU_FRAME_COUNTER);

  } else if (addr > MEMORY_ROM && mem->nes->cartridge != NULL) {
    return cartridge_read(mem->nes->cartridge, addr - MEMORY_ROM);
  }

  return 0;
}

void memory_write(Memory * mem, uint16_t addr, byte val) {
  if (addr < MEMORY_RAM_END) {
    mem->ram[addr % MEMORY_RAM_SIZE] = val;

  } else if (addr >= MEMORY_WAVEFORMS && addr < MEMORY_WAVEFORMS_END) {
    apu_write(mem->nes->apu, addr - MEMORY_WAVEFORMS, val);

  } else if (addr == MEMORY_APU_STATUS) {
    apu_write(mem->nes->apu, APU_STATUS, val);

  } else if (addr == MEMORY_APU_FRAME_COUNTER) {
    apu_write(mem->nes->apu, APU_FRAME_COUNTER, val);
  }
}
