#include <stdbool.h>

#include "pulse.h"
#include "length_table.h"

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/APU_Pulse
 */

struct Pulse {
  uint8_t duty           : 2;
  bool loop              : 1;
  bool envelope_disabled : 1;
  uint8_t volume         : 4;

  bool sweep_enabled     : 1;
  uint8_t sweep_period   : 3;
  bool sweep_negate      : 1;
  uint8_t sweep_shift    : 3;

  uint16_t period        : 11;
  uint8_t length         : 5;

  // Internal variables
  bool channel           : 1;

  bool envelope_reload   : 1;
  uint8_t envelope_val   : 4;

  bool sweep_reload      : 1;
  uint8_t sweep_timer    : 3;

  uint16_t period_timer  : 11;
  uint8_t phase          : 3;

  uint8_t length_timer   : 8;
};

static uint8_t pulse_sequencer[4][8] = {
  {0, 0, 0, 0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 1, 1},
  {0, 0, 0, 0, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 0}
};

void pulse_init(Pulse * pulse, uint8_t channel) {
  pulse->channel = channel;
}

uint8_t pulse_sample(Pulse * pulse) {
  if (!pulse->loop && pulse->length_timer == 0) {
    return 0;
  }

  if (!pulse_sequencer[pulse->duty][pulse->phase]) {
    return 0;
  }

  if (pulse->envelope_disabled) {
    return pulse->volume;
  } else {
    return pulse->envelope_val;
  }
}

void pulse_period_tick(Pulse * pulse) {
  if (pulse->period_timer != 0) {
    pulse->period_timer--;
  } else {
    pulse->phase++;
    pulse->period_timer = pulse->period;
  }
}

void pulse_length_tick(Pulse * pulse) {
  if (!pulse->loop && pulse->length_timer != 0) {
    pulse->length_timer--;
  }
}

static void pulse_sweep(Pulse * pulse) {
  uint16_t delta = pulse->period_timer >> pulse->sweep_shift;

  if (pulse->sweep_negate) {
    pulse->period_timer -= delta;
    if (pulse->channel == 1) { // pulse channel 1 hardwires carry = 0 on adder
      pulse->period_timer--;
    }
  } else {
    pulse->period_timer += delta;
  }
}

void pulse_sweep_tick(Pulse * pulse) {
  if (pulse->sweep_reload) {
    if (pulse->sweep_enabled && pulse->sweep_timer == 0) {
      pulse_sweep(pulse);
    }
    pulse->sweep_timer = pulse->sweep_period;
    pulse->sweep_reload = false;
  } else if (pulse->sweep_timer != 0) {
    pulse->sweep_timer--;
  } else if (pulse->sweep_enabled) {
    pulse_sweep(pulse);
    pulse->sweep_timer = pulse->sweep_period;
  }
}

void pulse_envelope_tick(Pulse * pulse) {
  if (pulse->envelope_reload) {
    pulse->envelope_val = 15;
    pulse->envelope_reload = false;
  } else if (pulse->envelope_val != 0) {
    pulse->envelope_val--;
  } else if (pulse->loop) {
    pulse->envelope_val = 15;
  }
}

void pulse_write(Pulse * pulse, uint8_t addr, uint8_t val) {
  switch (addr) {
  case 0:
    pulse->duty = (val >> 6) & 3;
    pulse->loop = (val >> 5) & 1;
    pulse->envelope_disabled = (val >> 4) & 1;
    pulse->volume = (val >> 0) & 15;
    break;
  case 1:
    pulse->sweep_enabled = (val >> 7) & 1;
    pulse->sweep_period = (val >> 4) & 7;
    pulse->sweep_negate = (val >> 3) & 1;
    pulse->sweep_shift = (val >> 0) & 7;
    pulse->sweep_reload = true;
    break;
  case 2:
    pulse->period = (pulse->period & 0x0700) | val;
    break;
  case 3:
    pulse->length = (val >> 3) & 31;
    pulse->period = (val & 0x07) << 8 | (pulse->period & 0x00FF);
    pulse->length_timer = length_table[pulse->length];
    pulse->envelope_reload = true;
    pulse->phase = 0;
    break;
  }
}

uint8_t pulse_read(Pulse * pulse, uint8_t addr) {
  uint8_t val = 0;

  switch (addr) {
  case 0:
    val |= (pulse->duty & 3) << 6;
    val |= (pulse->loop & 1) << 5;
    val |= (pulse->envelope_disabled & 1) << 4;
    if (pulse->envelope_disabled) {
      val |= (pulse->volume & 15) << 0;
    } else {
      val |= (pulse->envelope_val & 15) << 0;
    }
    break;
  case 1:
    val |= (pulse->sweep_enabled & 1) << 7;
    val |= (pulse->sweep_period & 7) << 4;
    val |= (pulse->sweep_negate & 1) << 3;
    val |= (pulse->sweep_shift & 7) << 0;
  case 2:
    val |= pulse->period & 0x00FF;
    break;
  case 3:
    val |= (pulse->length & 31) << 3;
    val |= (pulse->period & 0x0700) >> 8;
  }

  return val;
}
