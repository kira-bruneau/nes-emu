#ifndef APU_H
#define APU_H

#include <stdint.h>

#include "apu.h"
#include "pulse.h"
#include "triangle.h"
#include "noise.h"
#include "dmc.h"

/**
 * References:
 * APU: http://wiki.nesdev.com/w/index.php/APU
 * Frame Counter: http://wiki.nesdev.com/w/index.php/APU_Frame_Counter
 * Mixer: http://wiki.nesdev.com/w/index.php/APU_Mixer
 */

typedef struct APU APU;
struct APU {
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
    bool irq_inhibit : 1;

    // Internal variables
    bool interrupt   : 1;
    uint16_t clock   : 15; // Must hold up to 18640
  } frame_counter;
};

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

void apu_init(APU * apu);
void apu_reset(APU * apu);

void apu_tick(APU * apu);
float apu_sample(APU * apu);
void apu_write(APU * apu, APUAddress addr, uint8_t val);
uint8_t apu_read(APU * apu, APUAddress addr);

int apu_test_io(APU * apu);

#endif
