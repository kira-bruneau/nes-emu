#include <glib.h>
#include <assert.h>

#include "memory.h"

#define RAM_MIN 0x0000
#define RAM_MAX 0x2000
#define MIRROR_SIZE 0x0800

#define ROM_MIN 0x8000
#define ROM_MAX 0xFFFF

Memory * memory_new(Cartridge * cartridge) {
  Memory * mem = g_malloc(sizeof(Memory));
  mem->cartridge = cartridge;
  mem->ram = g_malloc(MIRROR_SIZE);
  return mem;
}

byte memory_read(Memory * mem, uint16_t addr) {
  if (addr < RAM_MAX) {
    // Obtain value from first mirror in RAM
    return mem->ram[addr % MIRROR_SIZE];
  } else if (addr > ROM_MIN) {
    // Obtain value from cartridge ROM
    assert(mem->cartridge != NULL);
    return cartridge_read(mem->cartridge, addr - ROM_MIN);
  }

  return 0;
}

uint16_t memory_read16(Memory * mem, uint16_t addr) {
  byte low = memory_read(mem, addr);
  byte high = memory_read(mem, addr + 1);
  return high << 8 | low;
}

void memory_write(Memory * mem, uint16_t addr, byte val) {
  if (addr < RAM_MAX) {
    // Set value to first mirror in RAM
    mem->ram[addr % MIRROR_SIZE] = val;
  }
}
