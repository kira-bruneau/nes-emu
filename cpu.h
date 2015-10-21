#ifndef CPU_H
#define CPU_H

#include "util.h"
#include "memory.h"

typedef struct CPU {
  uint16_t pc;
  byte sp;
  byte a, x, y;

  // Flags
  byte c : 1;
  byte z : 1;
  byte i : 1;
  byte d : 1;
  byte b : 1;
  /* byte e : 1; */
  byte v : 1;
  byte n : 1;

  Memory * mem;
} CPU;

// Intialization functions
CPU * cpu_new();
void cpu_reset(CPU * cpu);

// Program advancement functions
void cpu_next_instr(CPU * cpu);
byte cpu_next_8(CPU * cpu);
uint16_t cpu_next_16(CPU * cpu);

// Read and write functions for registers
uint16_t cpu_read_pc(CPU * cpu);
void cpu_write_pc(CPU * cpu, uint16_t val);
byte cpu_read_sp(CPU * cpu);
void cpu_write_sp(CPU * cpu, byte val);
byte cpu_read_a(CPU * cpu);
void cpu_write_a(CPU * cpu, byte val);
byte cpu_read_x(CPU * cpu);
void cpu_write_x(CPU * cpu, byte val);
byte cpu_read_y(CPU * cpu);
void cpu_write_y(CPU * cpu, byte val);
byte cpu_read_n(CPU * cpu);
void cpu_write_n(CPU * cpu, byte val);
byte cpu_read_v(CPU * cpu);
void cpu_write_v(CPU * cpu, byte val);
byte cpu_read_b(CPU * cpu);
void cpu_write_b(CPU * cpu, byte val);
byte cpu_read_d(CPU * cpu);
void cpu_write_d(CPU * cpu, byte val);
byte cpu_read_i(CPU * cpu);
void cpu_write_i(CPU * cpu, byte val);
byte cpu_read_z(CPU * cpu);
void cpu_write_z(CPU * cpu, byte val);
byte cpu_read_c(CPU * cpu);
void cpu_write_c(CPU * cpu, byte val);

#endif
