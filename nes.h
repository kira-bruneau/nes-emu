#ifndef NES_H
#define NES_H

#include "cpu.h"

typedef struct NES {
  CPU * cpu;
} NES;

NES * nes_new(GFile * rom_file);

#endif
