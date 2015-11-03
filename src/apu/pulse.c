#include "pulse.h"

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/APU_Pulse
 */

byte pulse_sample(Pulse * pulse) {
  if (pulse->length_counter_halt != 1 && pulse->length_counter_val == 0) {
    return 0;
  }

  // TODO: Output should depend on duty value
  if (pulse->timer_val > 1024) {
    return 15;
  } else {
    return 0;
  }
}

void pulse_tick(Pulse * pulse) {
  pulse->timer_val -= 1;
  if (pulse->length_counter_halt != 1 && pulse->length_counter_val != 0) {
    pulse->length_counter_val -= 1;
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
