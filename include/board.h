#ifndef BOARD_H_
#define BOARD_H_

#include "common.h"
#include "piece.h"


#define WIDTH  512
#define HEIGHT 512
#define GRID_X 8
#define GRID_Y 8
#define GRID_SIZE GRID_X * GRID_Y
#define CELL_X (WIDTH / GRID_X)
#define CELL_Y (HEIGHT / GRID_Y)
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
extern bool possible_moves[GRID_X * GRID_Y];
extern e_turn turn;
extern void reset_possible_moves(void);
extern int find_possible_moves(int x, int y, e_piece type);
extern void init_board(char* str);
#endif // !BOARD_H_

