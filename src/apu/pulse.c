#include "pulse.h"

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/APU_Pulse
 */

struct Pulse {
  byte duty              : 2;
  bool loop              : 1;
  bool envelope_disabled : 1;
  byte volume            : 4;

  bool sweep_enabled     : 1;
  byte sweep_period      : 3;
  bool sweep_negate      : 1;
  byte sweep_shift       : 3;

  uint16_t period        : 11;
  byte length            : 5;

  // Internal variables
  bool channel           : 1;

  bool envelope_reload   : 1;
  byte envelope_val      : 4;

  bool sweep_reload      : 1;
  byte sweep_timer       : 3;

  uint16_t period_timer  : 11;
  byte period_val        : 3;

  byte length_timer      : 8;
};

static byte pulse_sequencer[4][8] = {
  {0, 0, 0, 0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 1, 1},
  {0, 0, 0, 0, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 0}
};

byte pulse_sample(Pulse * pulse) {
  if (!pulse->loop && pulse->length_timer == 0) {
    return 0;
  }

  if (!pulse_sequencer[pulse->duty][pulse->period_val]) {
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
    pulse->period_val++;
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

void pulse_write(Pulse * pulse, byte addr, byte val) {
  (void)pulse;
  (void)addr;
  (void)val;
}

byte pulse_read(Pulse * pulse, byte addr) {
  (void)pulse;
  (void)addr;
  return 0;
}
