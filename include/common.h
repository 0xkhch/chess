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
#if defined(_WIN32)
#define get_bit(i, x, y) ((i >> (63 - ((y) * (GRID_Y) + (x)))) & 1ULL)
#define set_bit(i, x, y) ((i) |= (1ULL << (63 - ((y) * GRID_Y + (x)))))
#define unset_bit(i, x, y) (i &= ~(1ULL << (63 - ((y) * (GRID_Y) + (x)))))

#else
#define get_bit(i, x, y) ((i >> (63 - ((y) * (GRID_Y) + (x)))) & 1UL)
#define set_bit(i, x, y) (i |= (1UL << (63 - ((y) * (GRID_Y) + (x)))))
#define set_bit_offset(i, offset) (i |= (1UL << (offset)))

#define unset_bit(i, x, y) (i &= ~(1UL << (63 - ((y) * (GRID_Y) + (x)))))
#define unset_bit_offset(i, offset) (i &= ~(1UL << (offset)))
#endif // _WIN32

typedef struct last_move {
    // from
    int fx;
    int fy;
    
    // to
    int tx;
    int ty;

    e_piece type;
} last_move;

typedef enum {
    INTRO,
    MENU,
    END,
    PLAY,
    PROMO
} e_screen_state;

typedef struct global_state {
    int selected_x;
    int selected_y;

    int mouse_pos_x;
    int mouse_pos_y;
    
    int king_x; // current turn kings pos
    int king_y;

    int checking_x;
    int checking_y;


    int promo_x;
    int promo_y;

    uint64_t amount_checked;
    uint64_t checking_moves;

    uint64_t selected_moves;
    uint64_t heatmap;

    last_move prev;

    e_piece selected_type;
    e_state state;
    e_check checked;
    e_screen_state screen_state;

    bool turn;
    bool en_passant;
    bool castling;
    bool inside_option;
    bool draw;
} global_state_t;
#endif // !COMMON_H_
