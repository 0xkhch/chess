#include "control.h"

void control(Sound* move_sound, Sound* capture_sound, int x, int y) {
    // TODO: is there no better way to do this?
    switch (global.state) {
        case NONE: {
#ifdef DEBUG
            if (board[y * GRID_X + x] == 0) {
                global.state = NONE;
                global.selected_type = e_EMPTY;
                global.selected_x = 0;
                global.selected_y = 0;
            }
            else {
                global.state = SELECT;
                global.selected_x = x;
                global.selected_y = y;
                global.selected_type = board[y * GRID_X + x];
            }
#else
            if ((board[y * GRID_X + x] == 0 
                || (board[y * GRID_X + x] < 0 && global.turn == t_WHITE) 
                || (board[y * GRID_X + x] > 0 && global.turn == t_BLACK))) {
                global.state = NONE;
                global.selected_type = e_EMPTY;
                global.selected_x = 0;
                global.selected_y = 0;
                global.selected_moves = 0;
            }
            else if ((board[y * GRID_X + x] > 0 && global.turn == t_WHITE) 
                    || (board[y * GRID_X + x] < 0 && global.turn == t_BLACK)) {
                global.state = SELECT;
                global.selected_x = x;
                global.selected_y = y;
                global.selected_type = board[y * GRID_X + x];
            }
#endif /* ifdef DEBUG */
        } break;
        case SELECT: {
#ifdef DEBUG
            if (get_bit(global.selected_moves, x, y)) {
                global.state = NONE;
                board[y * GRID_X + x] != e_EMPTY ? PlaySound(*capture_sound) : PlaySound(*move_sound);
                board[y * GRID_X + x] = board[global.selected_y * GRID_X + global.selected_x];
                board[global.selected_y * GRID_X + global.selected_x] = e_EMPTY;
                global.turn = !global.turn;

                global.selected_type = e_EMPTY;
                global.selected_x = 0;
                global.selected_y = 0;
            }
            else {
                global.state = NONE;
            }
#else
            if ((global.selected_y == y && global.selected_x == x) && ((board[y * GRID_X + x] > 0 && global.turn == t_WHITE) 
                || (board[y * GRID_X + x] < 0 && global.turn == t_BLACK))) {
                global.state = NONE;
                global.selected_type = e_EMPTY;
                global.selected_x = 0;
                global.selected_y = 0;
                break;
            }
            if ((board[y * GRID_X + x] > 0 && global.turn == t_WHITE) 
                || (board[y * GRID_X + x] < 0 && global.turn == t_BLACK)) {
                global.state = SELECT;
                global.selected_x = x;
                global.selected_y = y;
                global.selected_type = board[y * GRID_X + x];
            }
            else if (!get_bit(global.selected_moves, x, y) && ((board[y * GRID_X + x] <= 0 && global.turn == t_WHITE) 
                    || (board[y * GRID_X + x] >= 0 && global.turn == t_BLACK))) {
                global.state = NONE;
                global.selected_type = e_EMPTY;
                global.selected_x = 0;
                global.selected_y = 0;
            }
            else if ((get_bit(global.selected_moves, x, y) 
                    && ((board[y * GRID_X + x] <= 0 && global.turn == t_WHITE) 
                    || (board[y * GRID_X + x] >= 0 && global.turn == t_BLACK)))) {
                global.state = NONE;
                board[y * GRID_X + x] != e_EMPTY ? PlaySound(*capture_sound) : PlaySound(*move_sound);
                board[y * GRID_X + x] = board[global.selected_y * GRID_X + global.selected_x];
                board[global.selected_y * GRID_X + global.selected_x] = e_EMPTY;
                global.turn = !global.turn;
                global.selected_type = e_EMPTY;
                global.selected_x = 0;
                global.selected_y = 0;
            }
#endif /* ifdef DEBUG */
        } break;
    }
}
