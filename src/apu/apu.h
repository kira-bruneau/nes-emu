#ifndef APU_H
#define APU_H

#include "util.h"

typedef struct NES NES;
typedef struct APU APU;

typedef enum {
  APU_WAVEFORMS = 0,
  APU_PULSE1 = 0,
  APU_PULSE1_END = 4,

  APU_PULSE2 = 4,
  APU_PULSE2_END = 8,

  APU_TRIANGLE = 8,
  APU_TRIANGLE_END = 12,

  APU_NOISE = 12,
  APU_NOISE_END = 16,

  APU_DMC = 16,
  APU_DMC_END = 20,
  APU_WAVEFORMS_END = 20,

  APU_STATUS = 20,
  APU_FRAME_COUNTER = 21,

  APU_ADDRESS_END
} APUAddress;

APU * apu_create(NES * nes);
void apu_destroy(APU * apu);

void apu_tick(APU * apu);
float apu_sample(APU * apu);
void apu_write(APU * apu, APUAddress addr, byte val);
byte apu_read(APU * apu, APUAddress addr);

int apu_test_io(APU * apu);

#endif
