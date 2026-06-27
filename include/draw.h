#ifndef DRAW_H_
#define DRAW_H_
#include "common.h"
void draw_board(Texture2D* pieces);
extern e_piece board[BOARD_SIZE];
extern uint64_t possible_moves;
extern global_state_t global;
#endif // !DRAW_H_
