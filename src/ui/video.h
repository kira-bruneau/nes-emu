#ifndef RENDER_H
#define RENDER_H

typedef struct Video Video;

Video * video_create(void);
void video_destroy(Video * video);
void video_loop(Video * video);

#endif
