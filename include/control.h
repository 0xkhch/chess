#ifndef CONTROL_H_
#define CONTROL_H_
#include "common.h"
#include "board.h"

void control(Sound* move_sound, Sound* capture_sound, int x, int y);
void clear_selection();
extern e_piece board[BOARD_SIZE];
extern e_piece last_board[BOARD_SIZE];
extern global_state_t global;
extern global_state_t previous;

#endif // !CONTROL_H_
