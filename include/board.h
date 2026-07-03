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
extern e_piece board[GRID_X * GRID_Y];
extern void reset_heatmap(bool type);
extern void find_selected_moves(global_state_t* global);
extern void init_board(char* str);
#endif // !BOARD_H_

