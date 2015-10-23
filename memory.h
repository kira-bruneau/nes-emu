#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#include "cartridge.h"
#include "util.h"

typedef struct Memory {
  byte * ram;
  Cartridge * cartridge;
} Memory;

Memory * memory_new();
void memory_reset(Memory * mem);
void memory_map_cartridge(Memory * mem, Cartridge * cartridge);

byte memory_read(Memory * mem, uint16_t addr);
uint16_t memory_read16(Memory * mem, uint16_t addr);
void memory_write(Memory * mem, uint16_t addr, byte val);
void memory_write16(Memory * mem, uint16_t addr, uint16_t val);

#endif
