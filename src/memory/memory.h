#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

/**
 * References:
 * NesDoc: http://nesdev.com/NESDoc.pdf - p.11
 * NesDev: http://wiki.nesdev.com/w/index.php/CPU_memory_map
 */

#define MEMORY_RAM 0x0000
#define MEMORY_RAM_END 0x1FFF
#define MEMORY_RAM_SIZE 0x0800

#define MEMORY_STACK 0x0100
#define MEMORY_STACK_END 0x01FF

#define MEMORY_WAVEFORMS 0x4000
#define MEMORY_WAVEFORMS_END 0x4013
#define MEMORY_APU_STATUS 0x4015
#define MEMORY_APU_FRAME_COUNTER 0x4017

#define MEMORY_CARTRIDGE 0x4020
#define MEMORY_CARTRIDGE_END 0xFFFF

typedef struct Memory Memory;
struct Memory {
  uint8_t ram[MEMORY_RAM_SIZE];
};

void memory_init(Memory * mem);
void memory_reset(Memory * mem);

uint8_t memory_read(Memory * mem, uint16_t addr);
void memory_write(Memory * mem, uint16_t addr, uint8_t val);

#endif
