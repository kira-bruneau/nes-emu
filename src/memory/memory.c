#include <string.h>

#include "memory.h"

struct Memory {
  byte * ram;
  Cartridge * cartridge;
};

Memory * memory_create(void) {
  Memory * mem = g_malloc(sizeof(Memory));
  mem->cartridge = NULL;
  mem->ram = g_malloc0(MEMORY_RAM_SIZE);
  return mem;
}

void memory_reset(Memory * mem) {
  memset(mem->ram, 0, MEMORY_RAM_SIZE);
}

void memory_map_cartridge(Memory * mem, Cartridge * cartridge) {
  mem->cartridge = cartridge;
  memset(mem->ram, 0, MEMORY_RAM_SIZE);
}

byte memory_read(Memory * mem, uint16_t addr) {
  if (addr < MEMORY_RAM_END) {
    return mem->ram[addr % MEMORY_RAM_SIZE];

  } else if (addr > MEMORY_ROM && mem->cartridge != NULL) {
    return cartridge_read(mem->cartridge, addr - MEMORY_ROM);
  }

  return 0;
}

void memory_write(Memory * mem, uint16_t addr, byte val) {
  if (addr < MEMORY_RAM_END) {
    mem->ram[addr % MEMORY_RAM_SIZE] = val;

  }
}
