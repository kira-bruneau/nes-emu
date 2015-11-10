#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "apu.h"

void apu_init(APU * apu, NES * nes) {
  apu_reset(apu);

  apu->nes = nes;
  pulse_init(&apu->pulse1, 0);
  pulse_init(&apu->pulse2, 1);
  triangle_init(&apu->triangle);
  noise_init(&apu->noise);
}

void apu_reset(APU * apu) {
  NES * nes = apu->nes;
  memset(apu, 0, sizeof(APU));
  apu->nes = nes;
}

float apu_sample(APU * apu) {
  uint8_t pulse1   = apu->status.pulse1   ? pulse_sample(&apu->pulse1)      : 0;
  uint8_t pulse2   = apu->status.pulse2   ? pulse_sample(&apu->pulse2)      : 0;
  uint8_t triangle = apu->status.triangle ? triangle_sample(&apu->triangle) : 0;
  uint8_t noise    = apu->status.noise    ? noise_sample(&apu->noise)       : 0;
  uint8_t dmc      = apu->status.dmc      ? dmc_sample(&apu->dmc)           : 0;

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

void apu_write(APU * apu, APUAddress addr, uint8_t val) {
  if (addr >= APU_PULSE1 && addr < APU_PULSE1_END) {
    pulse_write(&apu->pulse1, addr - APU_PULSE1, val);

  } else if (addr >= APU_PULSE2 && addr < APU_PULSE2_END) {
    pulse_write(&apu->pulse2, addr - APU_PULSE2, val);

  } else if (addr >= APU_TRIANGLE && addr < APU_TRIANGLE_END) {
    triangle_write(&apu->triangle, addr - APU_TRIANGLE, val);

  } else if (addr >= APU_NOISE && addr < APU_NOISE_END) {
    noise_write(&apu->noise, addr - APU_NOISE, val);

  } else if (addr >= APU_DMC && addr < APU_DMC_END) {
    dmc_write(&apu->dmc, addr - APU_DMC, val);

  } else if (addr == APU_STATUS) {
    apu->status.dmc = (val >> 4) & 1;
    apu->status.noise = (val >> 3) & 1;
    apu->status.triangle = (val >> 2) & 1;
    apu->status.pulse1 = (val >> 1) & 1;
    apu->status.pulse2 = (val >> 0) & 1;

  } else if (addr == APU_FRAME_COUNTER) {
    apu->frame_counter.mode = (val >> 7) & 1;
    apu->frame_counter.irq_inhibit = (val >> 6) & 1;
  }
}

uint8_t apu_read(APU * apu, APUAddress addr) {
  uint8_t val = 0;

  if (addr >= APU_PULSE1 && addr < APU_PULSE1_END) {
    val = pulse_read(&apu->pulse1, addr - APU_PULSE1);

  } else if (addr >= APU_PULSE2 && addr < APU_PULSE2_END) {
    val = pulse_read(&apu->pulse2, addr - APU_PULSE2);

  } else if (addr >= APU_TRIANGLE && addr < APU_TRIANGLE_END) {
    val = triangle_read(&apu->triangle, addr - APU_TRIANGLE);

  } else if (addr >= APU_NOISE && addr < APU_NOISE_END) {
    val = noise_read(&apu->noise, addr - APU_NOISE);

  } else if (addr >= APU_DMC && addr < APU_DMC_END) {
    val = dmc_read(&apu->dmc, addr - APU_DMC);

  } else if (addr == APU_STATUS) {
    val |= (apu->status.dmc & 1) << 4;
    val |= (apu->status.noise & 1) << 3;
    val |= (apu->status.triangle & 1) << 2;
    val |= (apu->status.pulse1 & 1) << 1;
    val |= (apu->status.pulse2 & 1) << 0;

  } else if (addr == APU_FRAME_COUNTER) {
    val |= (apu->frame_counter.mode & 1) << 7;
    val |= (apu->frame_counter.irq_inhibit & 1) << 6;
  }

  return val;
}

/**
 * Exaustive test to ensure that each write
 * correlates with the expected read
 */
int apu_test_io(APU * apu) {
  for (int addr = 0; addr < APU_ADDRESS_END; ++addr) {
    for (int val = 0; val < 256; ++val) {
      int compare = val;
      apu_write(apu, addr, val);

      // When envelope is on, assume volume = 0
      // In reality it should be value of envelope, but initially it's 0
      if ((addr == APU_PULSE1 ||
           addr == APU_PULSE2 ||
           addr == APU_NOISE) &&
          !(compare & 0b00010000)) {
        compare &= 0b11110000;
      }

      // Unused bit & bytes
      if (addr == APU_TRIANGLE + 1) {
        compare = 0;
      }

      if (addr == APU_NOISE) {
        compare &= 0b00111111;
      }

      if (addr == APU_NOISE + 1) {
        compare = 0;
      }

      if (addr == APU_NOISE + 2) {
        compare &= 0b10001111;
      }

      if (addr == APU_NOISE + 3) {
        compare &= 0b11111000;
      }

      // TODO: DMC not implemented
      if (addr >= APU_DMC && addr < APU_DMC_END) {
        compare = 0;
      }

      // TODO: interrupts not implemented
      if (addr == APU_STATUS) {
        compare &= 0b00011111;
      }

      if (addr == APU_FRAME_COUNTER) {
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
