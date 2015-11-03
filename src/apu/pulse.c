#include "pulse.h"

void pulse_tick(Pulse * pulse) {
  pulse->timer_val += 1;
}

byte pulse_sample(Pulse * pulse) {
  if (pulse->timer_val > 1024) {
    return 15;
  } else {
    return 0;
  }
}
