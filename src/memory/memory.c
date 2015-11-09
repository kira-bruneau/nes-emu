#include <string.h>

#include "memory.h"

struct Memory {
  byte * ram;
  Cartridge * cartridge;
};

Memory * memory_create(void) {
  Memory * mem = g_malloc(sizeof(Memory));
  mem->ram = g_malloc0(RAM_SIZE);
  mem->cartridge = NULL;
  return mem;
}

void memory_reset(Memory * mem) {
  memset(mem->ram, 0, RAM_SIZE);
}

void memory_map_cartridge(Memory * mem, Cartridge * cartridge) {
  mem->cartridge = cartridge;
}

byte memory_read(Memory * mem, uint16_t addr) {
  if (addr < RAM_MAX) {
    // Obtain value from first mirror in RAM
    return mem->ram[addr % RAM_SIZE];
  } else if (addr > ROM_MIN && mem->cartridge != NULL) {
    // Obtain value from cartridge ROM
    return cartridge_read(mem->cartridge, addr - ROM_MIN);
  }

  return 0;
}

void memory_write(Memory * mem, uint16_t addr, byte val) {
  if (addr < RAM_MAX) {
    // Write value to first mirror in RAM
    mem->ram[addr % RAM_SIZE] = val;
  }
}
