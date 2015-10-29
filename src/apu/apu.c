#include <stdio.h>
#include <stdlib.h>

#include "apu.h"

static float apu_pulse1(APU * apu) {
  if (!apu->status.pulse1) {
    return 0;
  }

  return pulse_output(&apu->pulse1);
}

static float apu_pulse2(APU * apu) {
  if (!apu->status.pulse2) {
    return 0;
  }

  return pulse_output(&apu->pulse1);
}

static float apu_triangle(APU * apu) {
  if (!apu->status.triangle) {
    return 0;
  }

  return triangle_output(&apu->triangle);
}

static float apu_noise(APU * apu) {
  if (!apu->status.noise) {
    return 0;
  }

  return noise_output(&apu->noise);
}

static float apu_dmc(APU * apu) {
  if (!apu->status.dmc) {
    return 0;
  }

  return dmc_output(&apu->dmc);
}

float apu_output(APU * apu) {
  float pulse_out = 0.00752 * (apu_pulse1(apu) + apu_pulse2(apu));
  float tnd_out = 0.00851 * apu_triangle(apu) + 0.00494 * apu_noise(apu) + 0.00335 * apu_dmc(apu);
  return pulse_out + tnd_out;
}
