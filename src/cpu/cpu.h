#ifndef CPU_H
#define CPU_H

#include <stdint.h>

/**
 * References:
 * CPU Wiki: http://wiki.nesdev.com/w/index.php/CPU
 * CPU Manual: http://users.telenet.be/kim1-6502/6502/proman.html
 * Opcodes: http://www.6502.org/tutorials/6502opcodes.html
 */

typedef struct CPU CPU;
struct CPU {
  int clock;

  uint16_t pc;
  uint8_t sp;
  uint8_t a, x, y;

  struct {
    uint8_t c : 1; // carry
    uint8_t z : 1; // zero
    uint8_t i : 1; // interrupt disable
    uint8_t d : 1; // bcd enable (ignored)
    uint8_t v : 1; // overflow
    uint8_t n : 1; // negative
  };
};

void cpu_init(CPU * cpu);
void cpu_reset(CPU * cpu);

void cpu_next_instr(CPU * cpu);
void cpu_debug(CPU * cpu);

#endif
