#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#include "cartridge.h"
#include "util.h"

typedef struct Memory {
  byte * raw;
  Cartridge * cartridge;
} Memory;

Memory * memory_new(Cartridge * cartridge, size_t size);
byte memory_read(Memory * mem, uint16_t addr);
uint16_t memory_read16(Memory * mem, uint16_t addr);
void memory_write(Memory * mem, uint16_t addr, byte val);

#endif
