#ifndef UI_H
#define UI_H

#include "nes.h"
#include "video.h"
#include "audio.h"

typedef struct UI UI;
struct UI {
  NES nes;
  Video video;
  Audio * audio;
};

void ui_init(UI * ui);
void ui_deinit(UI * ui);
int ui_run(UI * ui, Cartridge * cartridge);

#endif
