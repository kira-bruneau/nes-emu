#include <string.h>
#include <stdio.h>

#include "apu.h"

/**
 * References:
 * APU: http://wiki.nesdev.com/w/index.php/APU
 * Frame Counter: http://wiki.nesdev.com/w/index.php/APU_Frame_Counter
 * Mixer: http://wiki.nesdev.com/w/index.php/APU_Mixer
 */

void apu_init(APU * apu) {
  // Set everything to zero for now
  memset(apu, 0, sizeof(APU));
}

float apu_sample(APU * apu) {
  byte pulse1   = apu->status.pulse1   ? pulse_sample(&apu->pulse1)      : 0;
  byte pulse2   = apu->status.pulse2   ? pulse_sample(&apu->pulse2)      : 0;
  byte triangle = apu->status.triangle ? triangle_sample(&apu->triangle) : 0;
  byte noise    = apu->status.noise    ? noise_sample(&apu->noise)       : 0;
  byte dmc      = apu->status.dmc      ? dmc_sample(&apu->dmc)           : 0;

  float pulse_out = 0.00752 * (pulse1 + pulse2);
  float tnd_out = 0.00851 * triangle + 0.00494 * noise + 0.00335 * dmc;
  return pulse_out + tnd_out;
}

void apu_tick(APU * apu) {
  pulse_timer_tick(&apu->pulse1);
  pulse_timer_tick(&apu->pulse2);

  // Tick twice since triangle timer ticks at 2 * APU (CPU)
  triangle_timer_tick(&apu->triangle);
  triangle_timer_tick(&apu->triangle);

  noise_timer_tick(&apu->noise);

  if (apu->frame_counter.mode == 0) {
    // TODO:
    apu->frame_counter.clock += 1;
    if (apu->frame_counter.clock == 14915) {
      apu->frame_counter.clock = 0;
    }
  } else {
    // TODO:
    apu->frame_counter.clock += 1;
    if (apu->frame_counter.clock == 18641) {
      apu->frame_counter.clock = 0;
    }
  }
}
