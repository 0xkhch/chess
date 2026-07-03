#include "control.h"

static inline bool is_friendly(e_piece piece, bool turn);
static inline bool is_opposite(e_piece piece, bool turn);
void execute_move(Sound* move_sound, Sound* capture_sound, int x, int y);
void clear_selection();

void control(Sound* move_sound, Sound* capture_sound, int x, int y) {
    e_piece p = board_at(x, y);
    switch (global.state) {
        case NONE: {
#ifdef DEBUG
            if (is_empty(p)) {
                clear_selection();
                break;
            }
#else
            if (is_empty(p) || is_opposite(p, global.turn)) {
                clear_selection();
                break;
            }
#endif /* ifdef DEBUG */
            global.state = SELECT;
            global.selected_x = x;
            global.selected_y = y;
            global.selected_type = board[y * GRID_X + x];
        } break;
        case SELECT: {
            bool is_allowed = (bool)(get_bit(global.selected_moves, x, y));
#ifdef DEBUG
            is_allowed ? execute_move(move_sound, capture_sound, x, y) : clear_selection();
#else
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
#endif /* ifdef DEBUG */
        } break;
    }
}

void clear_selection()
{
    global.state = NONE;
    global.selected_type = e_EMPTY;
    global.selected_x = 0;
    global.selected_y = 0;
    global.selected_moves = 0;
}

void execute_move(Sound* move_sound, Sound* capture_sound, int x, int y)
{
#define p_pos(x, y) ((y*GRID_Y) + x)
    e_piece dst = board_at(x, y);
    dst != e_EMPTY ? PlaySound(*capture_sound) : PlaySound(*move_sound);
    board[p_pos(x, y)] = board[p_pos(global.selected_x, global.selected_y)];
    board[p_pos(global.selected_x, global.selected_y)] = e_EMPTY;
    global.turn = !global.turn;
    clear_selection();
}


static inline bool is_friendly(e_piece piece, bool turn)
{
    return (turn == t_WHITE && is_white(piece)) || (turn == t_BLACK && is_black(piece));
}

static inline bool is_opposite(e_piece piece, bool turn)
{
    return (turn == t_WHITE && is_black(piece)) || (turn == t_BLACK && is_white(piece));
}
