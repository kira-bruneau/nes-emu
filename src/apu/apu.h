#ifndef APU_H
#define APU_H

#include "pulse.h"
#include "triangle.h"
#include "noise.h"
#include "dmc.h"

#include "util.h"

typedef struct {
  Pulse pulse1;
  Pulse pulse2;
  Triangle triangle;
  Noise noise;
  DMC dmc;

  struct {
    bool dmc_interrupt   : 1; // read-only
    bool frame_interrupt : 1; // read-only
    bool dmc             : 1;
    bool noise           : 1;
    bool triangle        : 1;
    bool pulse2          : 1;
    bool pulse1          : 1;
  } status;

  struct {
    bool mode        : 1;
    bool irq_disable : 1; // 60hz interrupt
    // divider: CPU / 2
    // looping clock sequencer: keeps track of total APU cycles
  } frame_counter;
} APU;

void apu_init(APU * apu);
void apu_tick(APU * apu);
float apu_sample(APU * apu);

#endif
