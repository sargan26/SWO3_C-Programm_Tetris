#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "types.h"
#include <stdbool.h>

#define GB_ROWS 22 
#define GB_COLS 11

bool gb_is_valid_position(const position * pos);
void gb_update(const block * block);
void gb_render(void);

#endif
