#ifndef APU_H
#define APU_H

#include "util.h"

typedef struct APU APU;

APU * apu_create();
void apu_destroy(APU * apu);

void apu_tick(APU * apu);
float apu_sample(APU * apu);

#endif
