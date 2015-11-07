#ifndef CPU_H
#define CPU_H

#include "memory/memory.h"

typedef struct CPU CPU;

CPU * cpu_new(Memory * mem);
void cpu_reset(CPU * cpu);
void cpu_debug(CPU * cpu);

#endif
