#include <stdlib.h>

#include "noise.h"

void noise_tick(Noise * noise) {
  (void)noise;
}

byte noise_output(Noise * noise) {
  return rand() % 16;
}
