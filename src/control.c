#include "control.h"

bool is_friendly(e_piece piece, bool turn);
bool is_opposite(e_piece piece, bool turn);
bool is_enemy_king(e_piece piece, bool turn);
void execute_move(Sound* move_sound, Sound* capture_sound, int x, int y);
void clear_selection();

void control(Sound* move_sound, Sound* capture_sound, int x, int y) {
    e_piece p = board_at(x, y);
    switch (global.state) {
        case s_NONE: {
            if (is_empty(p) || is_opposite(p, global.turn)) {
                clear_selection();
                break;
            }
            global.state = s_SELECT;
            global.selected_x = x;
            global.selected_y = y;
            global.selected_type = board[y * GRID_X + x];
        } break;
        case s_SELECT: {
            bool is_allowed = (bool)(get_bit(global.selected_moves, x, y));
            if (x == global.selected_x && y == global.selected_y) {
                clear_selection();
                break;
            }
            if (is_friendly(p, global.turn)) {
                global.selected_x = x;
                global.selected_y = y;
                global.selected_type = p;
                break;
            }
            if (!is_allowed) {
                clear_selection();
                break;
            }
            execute_move(move_sound, capture_sound, x, y);
        } break;
    }
}

void clear_selection()
{
    global.state = s_NONE;
    global.selected_type = e_EMPTY;
    global.selected_x = 0;
    global.selected_y = 0;
    global.selected_moves = 0;
    global.heatmap = 0;
}

void execute_move(Sound* move_sound, Sound* capture_sound, int x, int y)
{
#define p_pos(x, y) ((y*GRID_Y) + x)
    e_piece dst = board_at(x, y);
    dst != e_EMPTY ? PlaySound(*capture_sound) : PlaySound(*move_sound);
    board[p_pos(x, y)] = board[p_pos(global.selected_x, global.selected_y)];
    board[p_pos(global.selected_x, global.selected_y)] = e_EMPTY;
    clear_selection();
    global.turn = !global.turn;
}


bool is_friendly(e_piece piece, bool turn)
{
    return (turn == t_WHITE && is_white(piece)) || (turn == t_BLACK && is_black(piece));
}

bool is_opposite(e_piece piece, bool turn)
{
    return (turn == t_WHITE && is_black(piece)) || (turn == t_BLACK && is_white(piece));
}
