#ifndef CPU_H
#define CPU_H

typedef struct CPU CPU;

CPU * cpu_new();
void cpu_reset(CPU * cpu);
void cpu_debug(CPU * cpu);

#endif
