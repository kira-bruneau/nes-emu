#include <glib.h>

#include "memory.h"

Memory * memory_new(Cartridge * cartridge, size_t size) {
  Memory * mem = g_malloc(sizeof(Memory));
  mem->cartridge = cartridge;
  mem->raw = g_malloc(size);
  return mem;
}

byte memory_read(Memory * mem, uint16_t addr) {
  if (addr < 0x2000) {
    return mem->raw[addr % 0x0800];
  } else if (addr > 0x8000) {
    return cartridge_read(mem->cartridge, addr - 0x8000);
  }

  return 0;
}

uint16_t memory_read16(Memory * mem, uint16_t addr) {
  byte low = memory_read(mem, addr);
  byte high = memory_read(mem, addr + 1);
  return high << 8 | low;
}

void memory_write(Memory * mem, uint16_t addr, byte val) {
  mem->raw[addr] = val;
}
