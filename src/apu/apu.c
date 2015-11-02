#include <string.h>
#include <stdio.h>

#include "apu.h"

void apu_init(APU * apu) {
  memset(apu, 0, sizeof(APU));

  apu->status.pulse1 = 1;
  apu->pulse1.timer = 10;
  /* apu->status.triangle = 1; */
  /* apu->triangle.timer = 1000; */
}

void apu_tick(APU * apu) {
  // Two different modes: apu->frame_counter.mode
  pulse_tick(&apu->pulse1);
  pulse_tick(&apu->pulse2);
  triangle_tick(&apu->triangle);
  noise_tick(&apu->noise);
  dmc_tick(&apu->dmc);
}

static byte apu_sample_pulse1(APU * apu) {
  if (!apu->status.pulse1) {
    return 0;
  }

  return pulse_sample(&apu->pulse1);
}

static byte apu_sample_pulse2(APU * apu) {
  if (!apu->status.pulse2) {
    return 0;
  }

  return pulse_sample(&apu->pulse1);
}

static byte apu_sample_triangle(APU * apu) {
  if (!apu->status.triangle) {
    return 0;
  }

  return triangle_sample(&apu->triangle);
}

static byte apu_sample_noise(APU * apu) {
  if (!apu->status.noise) {
    return 0;
  }

  return noise_sample(&apu->noise);
}

static byte apu_sample_dmc(APU * apu) {
  if (!apu->status.dmc) {
    return 0;
  }

  return dmc_sample(&apu->dmc);
}

float apu_sample(APU * apu) {
  float pulse1 = apu_sample_pulse1(apu);
  float pulse2 = apu_sample_pulse2(apu);
  float triangle = apu_sample_triangle(apu);
  float noise = apu_sample_noise(apu);
  float dmc = apu_sample_dmc(apu);

  float pulse_out = 0.00752 * (pulse1 + pulse2);
  float tnd_out = 0.00851 * triangle + 0.00494 * noise + 0.00335 * dmc;
  return pulse_out + tnd_out;
}
