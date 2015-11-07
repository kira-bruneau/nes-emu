#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

  apu_test_io(apu);

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

static void apu_period_tick(APU * apu) {
  pulse_period_tick(&apu->pulse1);
  pulse_period_tick(&apu->pulse2);

  // Tick twice since triangle timer ticks at 2 * APU (CPU)
  triangle_period_tick(&apu->triangle);
  triangle_period_tick(&apu->triangle);

  noise_period_tick(&apu->noise);
}

static void apu_half_frame_tick(APU * apu) {
  pulse_sweep_tick(&apu->pulse1);
  pulse_sweep_tick(&apu->pulse2);
  pulse_length_tick(&apu->pulse1);
  pulse_length_tick(&apu->pulse2);
  triangle_length_tick(&apu->triangle);
  noise_length_tick(&apu->noise);
}

static void apu_quarter_frame_tick(APU * apu) {
  pulse_envelope_tick(&apu->pulse1);
  pulse_envelope_tick(&apu->pulse2);
  triangle_linear_tick(&apu->triangle);
  noise_envelope_tick(&apu->noise);
}

/*
 * Waveform subunits are ticked at half APU intervals.
 * I assume this is to ensure ordering between period ticks
 * and frame counter ticks.
 *
 * Since I rounded down the clock values for the steps, the
 * frame counter must be ticked after the period.
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
        apu->frame_counter.clock++;
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
        apu->frame_counter.clock++;
      }
    }
  }
}

void apu_tick(APU * apu) {
  apu_period_tick(apu);
  apu_frame_counter_tick(apu);
}

void apu_write(APU * apu, byte addr, byte val) {
  switch (addr) {
  case 0: case 1: case 2: case 3:
    pulse_write(&apu->pulse1, addr, val);
    break;
  case 4: case 5: case 6: case 7:
    pulse_write(&apu->pulse2, addr - 4, val);
    break;
  case 8: case 9: case 10: case 11:
    triangle_write(&apu->triangle, addr - 8, val);
    break;
  case 12: case 13: case 14: case 15:
    noise_write(&apu->noise, addr - 12, val);
    break;
  case 16: case 17: case 18: case 19:
    dmc_write(&apu->dmc, addr - 16, val);
    break;
  case 20:
    apu->status.dmc = (val >> 4) & 1;
    apu->status.noise = (val >> 3) & 1;
    apu->status.triangle = (val >> 2) & 1;
    apu->status.pulse1 = (val >> 1) & 1;
    apu->status.pulse2 = (val >> 0) & 1;
    break;
  case 21:
    apu->frame_counter.mode = (val >> 7) & 1;
    apu->frame_counter.irq_inhibit = (val >> 6) & 1;
    break;
  }
}

byte apu_read(APU * apu, byte addr) {
  byte val = 0;

  switch (addr) {
  case 0: case 1: case 2: case 3:
    val = pulse_read(&apu->pulse1, addr);
    break;
  case 4: case 5: case 6: case 7:
    val = pulse_read(&apu->pulse2, addr - 4);
    break;
  case 8: case 9: case 10: case 11:
    val = triangle_read(&apu->triangle, addr - 8);
    break;
  case 12: case 13: case 14: case 15:
    val = noise_read(&apu->noise, addr - 12);
    break;
  case 16: case 17: case 18: case 19:
    val = dmc_read(&apu->dmc, addr - 16);
    break;
  case 20:
    val |= (apu->status.dmc & 1) << 4;
    val |= (apu->status.noise & 1) << 3;
    val |= (apu->status.triangle & 1) << 2;
    val |= (apu->status.pulse1 & 1) << 1;
    val |= (apu->status.pulse2 & 1) << 0;
    break;
  case 21:
    val |= (apu->frame_counter.mode & 1) << 7;
    val |= (apu->frame_counter.irq_inhibit & 1) << 6;
    break;
  }

  return val;
}

/**
 * Exaustive test to ensure that each write
 * correlates with the expected read
 */
int apu_test_io(APU * apu) {
  for (int addr = 0; addr <= 21; ++addr) {
    for (int val = 0; val <= 255; ++val) {
      int compare = val;
      apu_write(apu, addr, val);

      // When envelope is on, assume volume = 0
      if ((addr == 0 || addr == 4 || addr == 12) && !(compare & 0b00010000)) {
        compare &= 0b11110000;
      }

      // Unused addresses, any write should give read = 0
      if (addr == 9 || addr == 13) {
        compare = 0;
      }

      // Partially unused, any write to upper 2 bits = 0
      if (addr == 12) {
        compare &= 0b00111111;
      }

      // Partially unused
      if (addr == 14) {
        compare &= 0b10001111;
      }

      // Partially unused
      if (addr == 15) {
        compare &= 0b11111000;
      }

      // DMC not implemented, assume 0 for now
      if (addr >= 16 && addr <= 19) {
        compare = 0;
      }

      // Partially unused (DMC interrupt & frame interrupt not implemented yet)
      if (addr == 20) {
        compare &= 0b00011111;
      }

      // Partially unused
      if (addr == 21) {
        compare &= 0b11000000;
      }

      if (apu_read(apu, addr) != compare) {
        fprintf(stderr, "APU I/O test failed at addr: %i, val:%i\n", addr , val);
        return 0;
      }
    }
  }

  return 1;
}
