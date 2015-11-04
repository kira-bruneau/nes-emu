#include "pulse.h"

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/APU_Pulse
 */

struct Pulse {
  byte duty                : 2;
  bool length_counter_halt : 1;
  bool constant_volume     : 1;
  byte envelope            : 4;

  bool sweep_unit          : 1;
  byte period              : 3;
  bool negate              : 1;
  byte shift               : 3;

  uint16_t timer           : 11;
  byte length_counter      : 5;

  // Internal variables
  uint16_t timer_val       : 11;
  uint16_t sequence_val    : 3;
  byte length_counter_val  : 5;
};

// NOTE: Sequence counter on actual hardware counts down, but this counts up
static byte pulse_sequencer[4][8] = {
  {0, 0, 0, 0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 1, 1},
  {0, 0, 0, 0, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 0}
};

byte pulse_sample(Pulse * pulse) {
  // TODO: Figure out how sweep effects the timer and gate

  if (pulse->length_counter_halt != 1 && pulse->length_counter_val == 0) {
    return 0;
  }

  if (pulse_sequencer[pulse->duty][pulse->sequence_val] == 0) {
    return 0;
  }

  return pulse->envelope;
}

void pulse_timer_tick(Pulse * pulse) {
  if (pulse->timer_val == 0) {
    pulse->sequence_val += 1;
    pulse->timer_val = pulse->timer;
  } else {
    pulse->timer_val -= 1;
  }
}

void pulse_length_counter_tick(Pulse * pulse) {
  if (pulse->length_counter_halt != 1 && pulse->length_counter_val != 0) {
    pulse->length_counter_val -= 1;
  }
}

void pulse_sweep_tick(Pulse * pulse) {
  (void)pulse;
}

void pulse_envelope_tick(Pulse * pulse) {
  (void)pulse;
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
