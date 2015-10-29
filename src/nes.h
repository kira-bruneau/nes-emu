#ifndef NES_H
#define NES_H

#include "cartridge/cartridge.h"

typedef struct NES NES;

NES * nes_new(void);
void nes_load(NES * nes, Cartridge * cartridge);

#endif
