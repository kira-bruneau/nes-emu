#include <glib.h>
#include <string.h>
#include <assert.h>

#include "memory.h"

/**
 * References:
 * NesDev: http://wiki.nesdev.com/w/index.php/CPU_memory_map
 */

#define RAM_MIN 0x0000
#define RAM_MAX 0x2000
#define RAM_SIZE 0x0800

#define ROM_MIN 0x8000
#define ROM_MAX 0xFFFF

Memory * memory_new() {
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

uint16_t memory_read16(Memory * mem, uint16_t addr) {
  byte low = memory_read(mem, addr);
  byte high = memory_read(mem, addr + 1);
  return high << 8 | low;
}

void memory_write(Memory * mem, uint16_t addr, byte val) {
  if (addr < RAM_MAX) {
    // Write value to first mirror in RAM
    mem->ram[addr % RAM_SIZE] = val;
  }
}

void memory_write16(Memory * mem, uint16_t addr, uint16_t val) {
  byte low = val;
  byte high = val >> 8;
  memory_write(mem, addr, low);
  memory_write(mem, addr + 1, high);
}
