#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <stdint.h>
#include "raylib.h"
#include "piece.h"

#define WIDTH  512
#define HEIGHT 512
#define GRID_X 8
#define GRID_Y 8
#define GRID_SIZE GRID_X * GRID_Y
#define CELL_X (WIDTH / GRID_X)
#define CELL_Y (HEIGHT / GRID_Y)
#define BOARD_SIZE (GRID_X * GRID_Y)

// whose turn
#define t_WHITE false
#define t_BLACK true

// bit board stuff
#define get_bit(i, x, y) ((i >> (63 - (y * 8 + x))) & 1UL)

#define set_bit(i, x, y) (i |= (1UL << (63 - (y * 8 + x))))
#define set_bit_offset(i, offset) (i |= (1UL << (offset)))

#define unset_bit(i, x, y) (i &= ~(1UL << (63 - (y * 8 + x))))
#define unset_bit_offset(i, offset) (i &= ~(1UL << (offset)))

typedef struct global_state {
    int selected_x;
    int selected_y;
    int mouse_pos_x;
    int mouse_pos_y;
    int mouse_pos;
    e_piece selected_type;
    e_state state;
    bool turn;
    bool is_black_checked;
    bool is_white_checked;
} global_state_t;
#endif // !COMMON_H_
