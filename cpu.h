#ifndef CPU_H
#define CPU_H

#include "util.h"
#include "memory.h"

typedef struct CPU {
  uint16_t pc;
  byte sp;
  byte a, x, y;

  // CCR
  byte n : 1;
  byte v : 1;
  byte b : 1;
  byte d : 1;
  byte i : 1;
  byte z : 1;
  byte c : 1;

  Memory * mem;
} CPU;

CPU * cpu_new();
void cpu_reset(CPU * cpu);

byte cpu_read_pc(CPU * cpu);
uint16_t cpu_read_pc16(CPU * cpu);

void cpu_eval_next(CPU * cpu);
/* void cpu_eval_control(byte opcode); */
/* void cpu_eval_alu(byte opcode); */
/* void cpu_eval_rmw(byte opcode); */
/* void cpu_eval_unofficial(byte opcode) */

#endif
