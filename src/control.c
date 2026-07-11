#include "control.h"

bool is_friendly(e_piece piece, bool turn);
bool is_opposite(e_piece piece, bool turn);
bool is_enemy_king(e_piece piece, bool turn);
void execute_move(Sound* move_sound, Sound* capture_sound, int x, int y);
void clear_selection();
bool is_pawn(e_piece p);
bool is_king(e_piece p);

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
            find_selected_moves();
        } break;
        case s_SELECT: {
            bool is_allowed = (bool)(get_bit(global.selected_moves, x, y));
            if (x == global.selected_x && y == global.selected_y) {
                clear_selection();
                break;
            }
            if (is_friendly(p, global.turn)) {
                global.selected_moves = 0;
                global.selected_x = x;
                global.selected_y = y;
                global.selected_type = p;
                find_selected_moves();
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
    global.en_passant = false;
}

void execute_move(Sound* move_sound, Sound* capture_sound, int x, int y)
{
#define b_pos(x, y) ((y*GRID_Y) + x)
    e_piece dst = board_at(x, y);
    e_piece src = board_at(global.selected_x, global.selected_y);


    dst != e_EMPTY ? PlaySound(*capture_sound) : PlaySound(*move_sound);

    if (is_pawn(global.selected_type) && global.en_passant) {
        int direction = (global.selected_type == w_PAWN) ? 1 : -1;
        if (y == global.prev.fy + direction && x == global.prev.fx) {
            board[b_pos(global.prev.tx, global.prev.ty)] = e_EMPTY;
        }
    }

    if (is_king(global.selected_type) && global.castling) {
        if (x == (global.selected_x - 2)) {
            board[b_pos(global.selected_x - 4, y)] = e_EMPTY;
            board[b_pos(global.selected_x - 1, y)] = global.selected_type == w_KING ? w_ROOK : b_ROOK;
        }
        if (x == (global.selected_x + 2)) {
            board[b_pos(global.selected_x + 3, y)] = e_EMPTY;
            board[b_pos(global.selected_x + 1, y)] = global.selected_type == w_KING ? w_ROOK : b_ROOK;
        }
    }
    board[b_pos(x, y)] = src;
    board[b_pos(global.selected_x, global.selected_y)] = e_EMPTY;


    global.prev.type = src;
    global.prev.fx = global.selected_x;
    global.prev.fy = global.selected_y;

    global.prev.tx = x;
    global.prev.ty = y;

    global.turn = !global.turn;
    check();
    clear_selection();
}

bool is_pawn(e_piece p)
{
    return p == w_PAWN || p == b_PAWN;
}

bool is_king(e_piece p)
{
    return p == w_KING || p == b_KING;
}

bool is_friendly(e_piece piece, bool turn)
{
    return (turn == t_WHITE && is_white(piece)) || (turn == t_BLACK && is_black(piece));
}

bool is_opposite(e_piece piece, bool turn)
{
    return (turn == t_WHITE && is_black(piece)) || (turn == t_BLACK && is_white(piece));
}
