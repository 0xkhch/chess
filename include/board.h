#ifndef BOARD_H_
#define BOARD_H_

#include "common.h"
/*
   10 = white king
    9 = white queen
    5 = white rook
    4 = white bishop
    3 = white knight
    1 = white pawn

  -10 = black king
   -9 = black queen
   -5 = black rook
   -4 = black bishop
   -3 = black knight
   -1 = black pawn
*/

typedef struct {
    uint64_t heatmap;
    int amount_checked;
    int checking_x;
    int checking_y;
} heat_map;

#define BOARD_AT(board, x,y) board[(y) * GRID_X + (x)]
extern e_piece board[GRID_X * GRID_Y];
void reset_heatmap(bool type);
void find_selected_moves();
void check();

void load_board(char* str);
e_piece char_to_enum(char c);
char enum_to_char(e_piece p);
void set_turn(char c);

e_piece board_at(int x, int y);
bool is_white(e_piece p);
bool is_empty(e_piece p);
bool is_black(e_piece p);
bool is_enemy(e_piece a, e_piece b);
bool is_pawn(e_piece p);
bool is_king(e_piece p);
bool is_friendly(e_piece piece, bool turn);
bool is_opposite(e_piece piece, bool turn);
bool is_checked(e_piece type);

extern global_state_t global;

#endif // !BOARD_H_

