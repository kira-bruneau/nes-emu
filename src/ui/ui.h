#ifndef UI_H
#define UI_H

#include "nes.h"

typedef struct UI UI;

int ui_init(void);
void ui_terminate(void);
UI * ui_create(NES * nes);
void ui_destroy(UI * ui);
int ui_run(UI * ui);

#endif
