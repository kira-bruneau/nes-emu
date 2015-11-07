#include "clock.h"

int frequency_scale(float ratio, int clock) {
  int int_part = (int)ratio;
  float frac_part = ratio - int_part;

  int result = int_part;
  if ((int)(clock * frac_part) != (int)((clock + 1) * frac_part)) {
    result += 1;
  }

  return result;
}
