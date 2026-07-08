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
#define BOARD_AT(board, x,y) board[(y) * GRID_X + (x)]
extern e_piece board[GRID_X * GRID_Y];
extern void reset_heatmap(bool type);
extern void find_selected_moves();
extern void check();

extern void load_board(char* str);
extern e_piece char_to_enum(char c);
extern char enum_to_char(e_piece p);
extern void set_turn(char c);

extern e_piece board_at(int x, int y);
extern bool is_white(e_piece p);
extern bool is_empty(e_piece p);
extern bool is_black(e_piece p);

extern global_state_t global;

#endif // !BOARD_H_

