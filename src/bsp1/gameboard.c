#include <assert.h>
#include "gameboard.h"
#include "types.h"
#include "renderer.h"

#define MAX_BLOCK_COUNT 100

static
size_t count_blocks;

static
block blocks[MAX_BLOCK_COUNT];

bool gb_is_valid_position(const position * pos) {
	assert(pos);
	if (pos->x < 0 || pos->y < 0) return false;
	if (pos->x >= GB_COLS || pos->y >= GB_ROWS) return false;
	for (size_t i = 0; i < count_blocks; i++)
	{
		const position* tmp = &blocks[i].pos;
		if (tmp->x == pos->x && tmp->y == pos->y) //position already occupied
			return false;
	}
	return true;
}

void gb_update(const block* block) {
    assert(block);
    assert(gb_is_valid_position(&block->pos));
    assert(count_blocks + 1 < MAX_BLOCK_COUNT);
    blocks[count_blocks++] = *block;
}

void gb_render(void) {
    for (size_t i = 0; i < count_blocks; i++) {
        const block *current = blocks + i;

        // loop over the tetrimino matrix dimensions
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                // check if the current cell in the matrix is occupied
                if (current->matrix[row][col]) {
                    position cell_pos = {
                        current->pos.x + col,
                        current->pos.y - row // corrected the row direction
                    };
                    renderer_render(&cell_pos, current->color);
                }
            }
        }
    }
}