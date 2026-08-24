#ifndef DRAW_H_
#define DRAW_H_

#include "common.h"
#include "board.h"

void draw_board(Texture2D* pieces);
void draw_intro();
void draw_menu(Texture2D* pieces);
void draw_promo(Texture2D* pieces);
void draw_end(Texture2D* pieces);
void draw_error(Texture2D* pieces);
extern e_piece board[BOARD_SIZE];
extern global_state_t global;
#endif // !DRAW_H_
