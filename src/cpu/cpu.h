#ifndef CPU_H
#define CPU_H

typedef struct NES NES;
typedef struct CPU CPU;

CPU * cpu_create(NES * mem);
void cpu_reset(CPU * cpu);

void cpu_next_instr(CPU * cpu);
void cpu_debug(CPU * cpu);

#endif
