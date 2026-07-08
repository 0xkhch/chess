#ifndef CONTROL_H_
#define CONTROL_H_
#include "common.h"
#include "board.h"

void control(Sound* move_sound, Sound* capture_sound, int x, int y);
extern e_piece board[BOARD_SIZE];
extern global_state_t global;

#endif // !CONTROL_H_
