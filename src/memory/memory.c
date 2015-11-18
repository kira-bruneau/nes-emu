#include <string.h>
#include <assert.h>

#include "memory.h"

////////////////////////////////////////////////////////////////////////////////

#include "nes.h"

// Note: This assumes that the Memory can only exist within a NES.
// Maybe this coupling is too strong...
static NES * memory_nes(Memory * mem) {
  return (NES *)((char *)mem - offsetof(struct NES, mem));
}

static Cartridge * memory_cartridge(Memory * mem) {
  return memory_nes(mem)->cartridge;
}

static APU * memory_apu(Memory * mem) {
  return &memory_nes(mem)->apu;
}

////////////////////////////////////////////////////////////////////////////////

void memory_init(Memory * mem) {
  memory_reset(mem);
}

void memory_reset(Memory * mem) {
  memset(mem->ram, 0, MEMORY_RAM_SIZE);
}

uint8_t memory_read(Memory * mem, uint16_t addr) {
  if (addr <= MEMORY_RAM_END) {
    return mem->ram[addr % MEMORY_RAM_SIZE];

  } else if (addr >= MEMORY_WAVEFORMS && addr <= MEMORY_WAVEFORMS_END) {
    return apu_read(memory_apu(mem), APU_WAVEFORMS + (addr - MEMORY_WAVEFORMS));

  } else if (addr == MEMORY_APU_STATUS) {
    return apu_read(memory_apu(mem), APU_STATUS);

  } else if (addr == MEMORY_APU_FRAME_COUNTER) {
    return apu_read(memory_apu(mem), APU_FRAME_COUNTER);

  } else if (addr >= MEMORY_CARTRIDGE) {
    Cartridge * cartridge = memory_cartridge(mem);
    if (cartridge) {
      return cartridge_read(cartridge, addr);
    }
  } else {
    assert(false);
  }

  return 0;
}

void memory_write(Memory * mem, uint16_t addr, uint8_t val) {
  if (addr <= MEMORY_RAM_END) {
    mem->ram[addr % MEMORY_RAM_SIZE] = val;

  } else if (addr >= MEMORY_WAVEFORMS && addr <= MEMORY_WAVEFORMS_END) {
    apu_write(memory_apu(mem), APU_WAVEFORMS + (addr - MEMORY_WAVEFORMS), val);

  } else if (addr == MEMORY_APU_STATUS) {
    apu_write(memory_apu(mem), APU_STATUS, val);

  } else if (addr == MEMORY_APU_FRAME_COUNTER) {
    apu_write(memory_apu(mem), APU_FRAME_COUNTER, val);

  } else if (addr >= MEMORY_CARTRIDGE) {
    Cartridge * cartridge = memory_cartridge(mem);
    if (cartridge) {
      cartridge_write(cartridge, addr, val);
    }
  } else {
    assert(false);
  }
}
