#ifndef RENDER_H
#define RENDER_H

typedef struct Video Video;
struct Video {
  /* PPU * ppu; */
};

void video_init(Video * video);
void video_render(Video * video);

#endif
