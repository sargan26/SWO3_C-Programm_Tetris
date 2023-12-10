#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "timer.h"
#include "gameboard.h"
#include "renderer.h"

static
block current = { {GB_COLS / 2, GB_ROWS - 1}, color_red,  {1} };

static int block_count = 0;

// function to generate a random Tetrimino
static
block generate_random_tetrimino(void) {
    static const color tetrimino_colors[] = {
        color_red,
        color_green,
        color_blue,
        color_cyan,
        color_yellow,
        color_magenta,
        color_white,
    };

    static const bool tetriminos[][4][4] = {
        // I
        {{0, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 0, 0}},

        // J
        {{0, 1, 0, 0},
         {0, 1, 1, 0},
         {0, 0, 1, 0},
         {0, 0, 0, 0}},

        // L
        {{0, 0, 1, 0},
         {0, 1, 1, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 0}},

        // O
        {{0, 0, 0, 0},
         {0, 1, 1, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0}},

        // S
        {{0, 1, 1, 0},
         {1, 1, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},

        // T
        {{0, 1, 0, 0},
         {1, 1, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},

        // Z
        {{1, 1, 0, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}}
    };

    int random_index = rand() % 7;
    block tetrimino = {
        {GB_COLS / 2, GB_ROWS - 1},
        tetrimino_colors[random_index],
        {0}
    };

    // copy the tetrimino pattern
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            tetrimino.matrix[i][j] = tetriminos[random_index][i][j];
        }
    }

    return tetrimino;
}

void print_block(const block* blk) {
    printf("Position: (%d, %d)\n", blk->pos.x, blk->pos.y);
    printf("Color: %d\n", blk->color);

    printf("Matrix:\n");
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%d ", blk->matrix[i][j]);
        }
        printf("\n");
    }
}

static 
bool try_move(int dx, int dy) {
    block temp = current;
    temp.pos.x += dx;
    temp.pos.y += dy;

    // checks all positions in the matrix
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (temp.matrix[i][j]) {
                position cell_pos = {temp.pos.x + j, temp.pos.y - i};
                if (!gb_is_valid_position(&cell_pos)) {
                    return false;  // collision detected
                }
            }
        }
    }

    // no collision, update current block
    current = temp;
    return true;
}


// function to rotate the tetrimino clockwise
static
void rotate_tetrimino(void) {
    block temp = current;

    // transponate the matrix
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            int tmp = temp.matrix[i][j];
            temp.matrix[i][j] = temp.matrix[j][i];
            temp.matrix[j][i] = tmp;
        }
    }

    // mirrow the matrix
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 2; ++j) {
            int tmp = temp.matrix[i][j];
            temp.matrix[i][j] = temp.matrix[i][3 - j];
            temp.matrix[i][3 - j] = tmp;
        }
    }

    // check if roation was valid
    if (gb_is_valid_position(&temp)) {
        current = temp;
    }
}

static
void on_key(key_t key, action_t action) {
    int dx = 0, dy = 0;
    switch (key) {
        case key_up:
            if (action == action_press || action == action_repeat) {
                rotate_tetrimino();
            }
            break;
        case key_down: dy = -1; break;
        case key_left: dx = -1; break;
        case key_right: dx = +1; break;
        default: return; // nothing to do
    }

    if (action == action_press || action == action_repeat) {
        if (!try_move(dx, dy)) {
            if (dy == -1) { // tried to move down, but couldn't => must have hit bottom
                gb_update(&current);
                current = generate_random_tetrimino();
                block_count++;
                if (block_count % 10 == 0)
                {
                    timer_speedup();
                }
            }
        }
    }
}

static
void on_timer(void) {
    on_key(key_down, action_press); // simulate keyboard event - simplifies internal logic...
}

int main() {
    renderer_init(GB_ROWS, GB_COLS, on_key);
    timer_init(on_timer);

    current = generate_random_tetrimino(); // initialize the first Tetrimino
    print_block(&current);

    while (renderer_open()) {
        timer_test();
        renderer_begin_frame();

        // render the tetrimino matrix
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (current.matrix[i][j]) {
                    position cell_pos = {
                        current.pos.x + j,
                        current.pos.y - i
                    };

                    // create a temp pointer to const position
                    const position* cell_pos_ptr = &cell_pos;

                    renderer_render(cell_pos_ptr, current.color);
                }
            }
        }
        gb_render();

        renderer_end_frame();
    }

    renderer_shutdown();
}
