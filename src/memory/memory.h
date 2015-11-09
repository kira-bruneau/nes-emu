#ifndef MEMORY_H
#define MEMORY_H

#include "cartridge/cartridge.h"
#include "util.h"

/**
 * References:
 * NesDoc: http://nesdev.com/NESDoc.pdf - p.11
 * NesDev: http://wiki.nesdev.com/w/index.php/CPU_memory_map
 */

#define RAM_MIN 0x0000
#define RAM_MAX 0x2000
#define RAM_SIZE 0x0800

#define ZERO_PAGE_MIN 0x0000
#define ZERO_PAGE_MAX 0x0100

#define STACK_MIN 0x0100
#define STACK_MAX 0x0200

#define ROM_MIN 0x8000
#define ROM_MAX 0xFFFF

typedef struct Memory Memory;

Memory * memory_create(void);
void memory_reset(Memory * mem);
void memory_map_cartridge(Memory * mem, Cartridge * cartridge);

byte memory_read(Memory * mem, uint16_t addr);
void memory_write(Memory * mem, uint16_t addr, byte val);

#endif
