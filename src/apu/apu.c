#include <stdlib.h>
#include <string.h>

#include "apu.h"

// APU has tight coupling with sub components
// I could put all this code in apu.c,
// but I prefer to keep the seperation
#include "pulse.c"
#include "triangle.c"
#include "noise.c"
#include "dmc.c"

/**
 * References:
 * APU: http://wiki.nesdev.com/w/index.php/APU
 * Frame Counter: http://wiki.nesdev.com/w/index.php/APU_Frame_Counter
 * Mixer: http://wiki.nesdev.com/w/index.php/APU_Mixer
 */

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

APU * apu_create() {
  // Set everything to zero for now
  APU * apu = calloc(1, sizeof(APU));

  apu->status.pulse1 = 0;
  pulse_init(&apu->pulse1, 0);

  apu->status.pulse2 = 0;
  pulse_init(&apu->pulse2, 1);

  apu->status.triangle = 0;
  triangle_init(&apu->triangle);

  apu->status.noise = 1;
  noise_init(&apu->noise);

  return apu;
}

void apu_destroy(APU * apu) {
  free(apu);
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

static void apu_timers_tick(APU * apu) {
  pulse_period_tick(&apu->pulse1);
  pulse_period_tick(&apu->pulse2);

  // Tick twice since triangle timer ticks at 2 * APU (CPU)
  triangle_period_tick(&apu->triangle);
  triangle_period_tick(&apu->triangle);

  noise_timer_tick(&apu->noise);
}

static void apu_half_frame_tick(APU * apu) {
  pulse_sweep_tick(&apu->pulse1);
  pulse_sweep_tick(&apu->pulse2);
  pulse_length_tick(&apu->pulse1);
  pulse_length_tick(&apu->pulse2);
  triangle_length_tick(&apu->triangle);
  noise_length_counter_tick(&apu->noise);
}

static void apu_quarter_frame_tick(APU * apu) {
  pulse_envelope_tick(&apu->pulse1);
  pulse_envelope_tick(&apu->pulse2);
  triangle_linear_tick(&apu->triangle);
  noise_envelope_tick(&apu->noise);
}

/*
 * Waveform subunits are ticked at half APU intervals.
 * I assume this is to ensure ordering between timer ticks
 * and subunit ticks.
 *
 * Since I rounded down the clock values for the steps, the
 * frame counter must be ticked after the timers.
 */
static void apu_frame_counter_tick(APU * apu) {
  if (apu->frame_counter.mode == 0) {
    switch (apu->frame_counter.clock) {
    case 14914:
      // TODO: Set IRQ
    case 7456:
      apu_half_frame_tick(apu);
    case 11185:
    case 3728:
      apu_quarter_frame_tick(apu);
    default:
      if (apu->frame_counter.clock == 14914) {
        apu->frame_counter.clock = 0;
      } else {
        apu->frame_counter.clock += 1;
      }
    }
  } else {
    switch (apu->frame_counter.clock) {
    case 18640:
    case 7456:
      apu_half_frame_tick(apu);
    case 11185:
    case 3728:
      apu_quarter_frame_tick(apu);
    default:
      if (apu->frame_counter.clock == 18640) {
        apu->frame_counter.clock = 0;
      } else {
        apu->frame_counter.clock += 1;
      }
    }
  }
}

void apu_tick(APU * apu) {
  apu_timers_tick(apu);
  apu_frame_counter_tick(apu);
}
