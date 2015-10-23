#ifndef CPU_H
#define CPU_H

#include "util.h"
#include "memory.h"

typedef struct CPU {
  uint16_t pc;
  byte sp;
  byte a, x, y;

  union {
    byte status;
    struct {
      byte c : 1;
      byte z : 1;
      byte i : 1;
      byte d : 1;
      byte b : 1;
      byte e : 1;
      byte v : 1;
      byte n : 1;
    };
  };

  Memory * mem;
} CPU;

CPU * cpu_new();
void cpu_next_instr(CPU * cpu);

void cpu_test_automated(CPU * cpu);
void cpu_test_interactive(CPU * cpu);

#endif
