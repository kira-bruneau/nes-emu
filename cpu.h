#ifndef CPU_H
#define CPU_H

#include "util.h"
#include "memory.h"

typedef struct CPU CPU;

CPU * cpu_new();
void cpu_reset(CPU * cpu);
void cpu_eval_next(CPU * cpu);

// Register write and read functions
byte cpu_read_pc(CPU * cpu);
uint16_t cpu_read_pc16(CPU * cpu);
void cpu_write_pc16(CPU * cpu, uint16_t val);
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
