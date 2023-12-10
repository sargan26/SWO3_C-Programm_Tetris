#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>
#include "types.h"

typedef enum {
	key_down,
	key_left,
	key_right,
	key_up,
	key_q,
	key_e,
} key_t;

typedef enum {
	action_press,
	action_repeat,
	action_release,
} action_t;

typedef void(*key_callback)(key_t, action_t);


void renderer_init(int rows, int cols, key_callback callback);
void renderer_shutdown(void);

bool renderer_open(void);

void renderer_begin_frame(void);
void renderer_end_frame(void);

void renderer_render(const position * pos, color color);

#endif
