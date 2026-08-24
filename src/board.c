#include "board.h"
#include "piece.h"
#include "stdlib.h"

e_piece board[BOARD_SIZE] = {0};
e_piece last_board[BOARD_SIZE] = {0};

bool generating_attacks = false; // hack fix for pawns

void move_pawn(int x,   int y, e_piece type, uint64_t* moves);
void move_knight(int x, int y, e_piece type, uint64_t* moves);
void move_bishop(int x, int y, e_piece type, uint64_t* moves);
void move_rook(int x,   int y, e_piece type, uint64_t* moves);
void move_king(int x,   int y, e_piece type, uint64_t* moves);
void find_moves(int x, int y, e_piece type, uint64_t* moves);
void print_board(uint64_t x);
e_piece char_to_enum(char c);
char enum_to_char(e_piece p);
void set_turn(char c);
bool in_single_check(e_piece p);
bool is_slider(e_piece a);
bool is_enemy(e_piece a, e_piece b);
void sq2sq_moves_slider(e_piece type, int fx, int fy, int tx, int ty, uint64_t* moves);
bool in_single_check_by_slider(e_piece p);
bool can_castle_right(int x, int y, e_piece type);
bool can_castle_left(int x, int y, e_piece type);
bool check_pinned(int x, int y, e_piece type);

void find_selected_moves() {
    global.selected_pinned = check_pinned(global.selected_x, global.selected_y, global.selected_type);
    find_moves(global.selected_x, global.selected_y, global.selected_type, &global.selected_moves);
}

void find_moves(int x, int y, e_piece type, uint64_t* moves)
{
    switch (type) {
        case w_PAWN:
        case b_PAWN:
            move_pawn(x, y, type, moves);
        break;
        case w_KNIGHT:
        case b_KNIGHT:
            move_knight(x, y, type, moves);
        break;
        case w_BISHOP: 
        case b_BISHOP:
            move_bishop(x, y, type, moves);
        break;
        case w_ROOK:
        case b_ROOK:
            move_rook(x, y, type, moves);
        break;
        case w_QUEEN:
        case b_QUEEN:
            move_rook(x, y, type, moves);
            move_bishop(x, y, type, moves);
        break;

        case w_KING: 
        case b_KING:
            move_king(x, y, type, moves);
        break;

        default:
        break;
    }
}

int abs(int a)
{
    return a < 0 ? -a : a;
}

int sign(int a)
{
    return a == 0 ? 0 : (a > 0 ? 1 : -1);
}

bool check_pinned(int x, int y, e_piece type)
{
    int dx = x - global.king_x;
    int dy = y - global.king_y;
    if (dx == 0 && dy == 0) { // king cant be pinned
        return false;
    }
    if (dx != 0 && dy != 0 && abs(dx) != abs(dy)) {
        return false;
    }
    dx = sign(dx);
    dy = sign(dy);
    uint64_t pin_line = 0;

    // start at the square right after the king in direction of selected piece
    int p_x = global.king_x + dx;
    int p_y = global.king_y + dy;
    // check if there are any pieces between the king and selected piece
    while (p_x != x || p_y != y) {
        if (!is_empty(board_at(p_x, p_y))) {
            return false;
        }
        set_bit(pin_line, p_x, p_y);
        p_x = p_x + dx;
        p_y = p_y + dy;
    }

    if (p_x == x && p_y == y) {
        p_x = p_x + dx;
        p_y = p_y + dy;
    }

    while(p_x >= 0 && p_x < GRID_X && p_y >= 0 && p_y < GRID_Y) {
        e_piece p = board_at(p_x, p_y);
        if (!is_empty(p)) {
            if (is_enemy(p, type)) {
                if (is_slider(p)) {
                    global.pinned_by = p;
                    global.pinned_by_x = p_x;
                    global.pinned_by_y = p_y;
                    global.pin_line = pin_line;
                    return true;
                }
                return false;
            }
            return false;
        }
        set_bit(pin_line, p_x, p_y);
        p_x = p_x + dx;
        p_y = p_y + dy;
    }
    return false;
}

bool en_passantable(int x, int y, e_piece type)
{
    if (is_enemy(type, global.prev.type) && (global.prev.type == b_PAWN || global.prev.type == w_PAWN)) {
        int dy = y - global.king_y;
        int dx = x - global.king_x;
        if (dy == 0) { // on the same rank
            dx = sign(dx);
            int p_x = global.king_x + dx;
            int inbetween = 0;
            while (p_x != x) {
                if (!is_empty(board_at(p_x, global.king_y))) {
                    inbetween++;
                }
                p_x = p_x + dx;
            }
            if (inbetween == 1) {
                return false;
            }
        }
        if (y == 3 && abs((global.prev.ty - global.prev.fy)) == 2 && ((global.prev.tx == (x - 1)) || (global.prev.tx == (x + 1)))) {
            return true;
        }
        if (y == 4 && abs((global.prev.ty - global.prev.fy)) == 2 && ((global.prev.tx == (x - 1)) || (global.prev.tx == (x + 1)))) {
            return true;
        }
    }
    return false;
}

void move_if_valid(int x, int y, e_piece type, uint64_t* moves)
{
    set_bit(*moves, x, y);
    if (in_single_check_by_slider(type)) {
        if (!get_bit(global.checking_moves, x, y)) {
            unset_bit(*moves, x, y);
        }
    }
    if (global.selected_pinned && (get_bit(global.pin_line, x, y) == 0)) {
        unset_bit(*moves, x, y);
    }
}

void capture_if_valid(int x, int y, e_piece type, uint64_t* moves)
{
    set_bit(*moves, x, y);
    if (in_single_check(type)) {
        if (x != global.checking_x || y != global.checking_y) {
            if (!(global.checked && global.en_passant)) {
                unset_bit(*moves, x, y);
            }
        }
    }
    if (global.selected_pinned && (get_bit(global.pin_line, x, y) == 0)) {
        unset_bit(*moves, x, y);
    }
}

void move_pawn(int x, int y, e_piece type, uint64_t* moves)
{
    if (type == w_PAWN) {
        if (generating_attacks) {
            if (x != GRID_X - 1 && y != 0) set_bit(*moves, x + 1, y - 1);
            if (x != 0 && y != 0)          set_bit(*moves, x - 1, y - 1);
        } else {
            if (is_empty(board_at(x, y - 1))) {
                move_if_valid(x, y - 1, type, moves);
                if (is_empty(board_at(x, y - 2)) && y == 6) {
                    move_if_valid(x, y - 2, type, moves);
                }
            }
            if (x != GRID_X - 1 && (is_black(board_at(x + 1, y - 1)) || is_white(board_at(x + 1, y - 1)))) {
                capture_if_valid(x + 1, y - 1, type, moves);
            }
            if (x != 0 && (is_black(board_at(x - 1, y - 1)) || is_white(board_at(x - 1, y - 1)))) {
                capture_if_valid(x - 1, y - 1, type, moves);
            }
            if (en_passantable(x, y, type)) {
                global.prev.tx == (x - 1) ? capture_if_valid(x - 1, y - 1, type, moves) : capture_if_valid(x + 1, y - 1, type, moves);
                global.en_passant = true;
            }
        }
    }
    else if (type == b_PAWN) {
        if (generating_attacks) {
            if (x != GRID_X - 1 && y != GRID_Y - 1) set_bit(*moves, x + 1, y + 1);
            if (x != 0 && y != GRID_Y - 1)          set_bit(*moves, x - 1, y + 1);
        } else {
            if (is_empty(board_at(x, y + 1))) {
                move_if_valid(x, y + 1, type, moves);
                if (is_empty(board_at(x, y + 2)) && y == 1) {
                    move_if_valid(x, y + 2, type, moves);
                }
            }
            if (x != GRID_X - 1 && (is_white(board_at(x + 1, y + 1)) || is_black(board_at(x + 1, y + 1)))) {
                capture_if_valid(x + 1, y + 1, type, moves);
            }
            if (x != 0 && (is_white(board_at(x - 1, y + 1)) || is_black(board_at(x - 1, y + 1)))) {
                capture_if_valid(x - 1, y + 1, type, moves);
            }
            if (en_passantable(x, y, type)) {
                global.en_passant = true;
                global.prev.tx == (x - 1) ? capture_if_valid(x - 1, y + 1, type, moves) : capture_if_valid(x + 1, y + 1, type, moves);
            }
        }
    }
}

void move_square(int x, int y, int dx, int dy, e_piece type, uint64_t* moves)
{
    for (int i = y - dy; i <= y + dy; i = i + (dy * 2)) {
        for (int j = x - dx; j <= x + dx; j = j + (dx * 2)) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            e_piece p = board_at(j, i);
            if (is_empty(p)) {
                if (in_single_check(type)) {
                    bool possible = (bool)get_bit(global.checking_moves, j, i);
                    if (!is_slider(board_at(global.checking_x, global.checking_y)) || !possible) {
                        continue;
                    }
                }
                set_bit(*moves, j, i);
            }
            if (is_enemy(type, p)) {
                if (in_single_check(type)) {
                    if (j != global.checking_x || i != global.checking_y) {
                        continue;
                    }
                }
                set_bit(*moves, j, i);
            }
            else {
                set_bit(*moves, j, i);
            }
            if (global.selected_pinned && (get_bit(global.pin_line, j, i) == 0)) {
                unset_bit(*moves, j, i);
            }
        }
    }
}
void move_knight(int x, int y, e_piece type, uint64_t* moves)
{
    move_square(x, y, 1, 2, type, moves); // wide
    move_square(x, y, 2, 1, type, moves); // long
}


void move_axis(int x, int y, int dir_x, int dir_y, e_piece type, uint64_t* moves)
{
    int dx = x + dir_x;
    int dy = y + dir_y;
    while (dx >= 0 && dx < GRID_X && dy >= 0 && dy < GRID_Y) {
        e_piece p = board_at(dx, dy);
        if (is_empty(p)) {
            if (in_single_check(type)) {
                bool possible = (bool)get_bit(global.checking_moves, dx, dy);
                if (!is_slider(board_at(global.checking_x, global.checking_y)) || !possible) {
                    dx = dx + dir_x;
                    dy = dy + dir_y;
                    continue;
                }
            }
            if (global.selected_pinned && (get_bit(global.pin_line, dx, dy) == 0)) {
                dx = dx + dir_x;
                dy = dy + dir_y;
                continue;
            }
            set_bit(*moves, dx, dy);
        }
        else {
            if (is_enemy(type, p)) {
                if (in_single_check(type)) {
                    if (dx != global.checking_x || dy != global.checking_y) {
                        break; // if we can capture the piece but its not the one checking us then ignore it
                    }
                }
                set_bit(*moves, dx, dy);
            }
            else {
                set_bit(*moves, dx, dy); // set the bit of our ally so that we can check for draws
            }
            break;
        }
        dx = dx + dir_x;
        dy = dy + dir_y;
    }

}

void move_bishop(int x, int y, e_piece type, uint64_t* moves)
{
    move_axis(x, y,  1, -1, type, moves); // NE
    move_axis(x, y,  1,  1, type, moves); // SE
    move_axis(x, y, -1,  1, type, moves); // SW
    move_axis(x, y, -1, -1, type, moves); // NW
}

void move_rook(int x, int y, e_piece type, uint64_t* moves)
{
    move_axis(x, y,  1,  0, type, moves); // right
    move_axis(x, y,  0,  1, type, moves); // down
    move_axis(x, y, -1,  0, type, moves); // lefr
    move_axis(x, y,  0, -1, type, moves); // up
}

void sq2sq_moves_slider(e_piece type, int fx, int fy, int tx, int ty, uint64_t* moves)
{
    uint64_t amount_checked = global.amount_checked;
    global.amount_checked = 0;
    int dir_x = tx - fx > 0 ? 1 : -1; 
    if (fx == tx) {
        dir_x = 0;
    }
    int dir_y = ty - fy > 0 ? 1 : -1; 
    if (ty == fy) {
        dir_y = 0;
    }
    move_axis(fx, fy, dir_x, dir_y, type, moves);
    global.amount_checked = amount_checked;
}

bool find_piece_position(int* x, int* y, e_piece type)
{
    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            e_piece p = board_at(j, i);
            if (p == type) {
                *x = j;
                *y = i;
                return true;
            }
        }
    }
    return false;
}

heat_map build_heatmap(e_piece type)
{
    heat_map heatmap = {};
    uint64_t local = 0;
    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            e_piece p = board_at(j, i);
            if (is_enemy(type, p)) {
                generating_attacks = true;
                find_moves(j, i, p, &local);
                generating_attacks = false;
                bool heat = (bool)get_bit(local, global.king_x, global.king_y);
                if (heat) {
                    heatmap.checking_x = j;
                    heatmap.checking_y = i;
                    heatmap.amount_checked++;
                }
                heatmap.heatmap = heatmap.heatmap | local;
                local = 0;
            }
        }
    }
    return heatmap;
}

void check()
{
    int x = 0, y = 0;
    bool found = find_piece_position(&x, &y, global.turn == t_WHITE ? w_KING : b_KING);
    if (found) {
        global.king_x = x;
        global.king_y = y;

        heat_map enemy = build_heatmap(global.turn == t_WHITE ? w_KING : b_KING);

        global.amount_checked = enemy.amount_checked;
        global.checking_x = enemy.checking_x;
        global.checking_y = enemy.checking_y;
        global.heatmap = enemy.heatmap;

        bool heat = (bool)get_bit(enemy.heatmap, x, y);
        global.checked = (heat && global.turn == t_WHITE) ? c_WHITE : c_BLACK;
        global.amount_checked = heat ? global.amount_checked : 0;
        if (global.amount_checked > 1 && global.state == s_SELECT) { // only way is to move king
            if (global.selected_type != (global.turn == t_WHITE ? w_KING : b_KING)) {
                global.state = s_NONE;
                global.selected_type = e_EMPTY;
                global.selected_x = 0;
                global.selected_y = 0;
                global.selected_moves = 0;
                return;
            }
        }

        if (global.amount_checked == 1) {
            e_piece checked_by = board_at(global.checking_x, global.checking_y);
            sq2sq_moves_slider(checked_by, global.checking_x, global.checking_y, global.king_x, global.king_y, &global.checking_moves);
        }

        if (global.amount_checked > 0) {
            global.castling = false;
        }

        bool has_move = false;
        for (int i = 0; i < GRID_Y && !has_move; i++) {
            for (int j = 0; j < GRID_X && !has_move; j++) {
                e_piece p = board_at(j, i);
                if (!is_friendly(p, global.turn)) continue;

                uint64_t m = 0;
                bool saved_pinned = global.selected_pinned;
                uint64_t saved_line = global.pin_line;
                global.selected_pinned = check_pinned(j, i, p);
                find_moves(j, i, p, &m);
                global.selected_pinned = saved_pinned;
                global.pin_line = saved_line;

                // mask out ally-occupied squares (move_axis stamps them)
                for (int a = 0; a < GRID_Y; a++)
                    for (int b = 0; b < GRID_X; b++)
                        if (!is_empty(board_at(b, a)) &&
                            !is_enemy(p, board_at(b, a)) &&
                            !(b == j && a == i))
                            unset_bit(m, b, a);

                if (m != 0) has_move = true;
            }
        }

        if (!has_move) {
            global.screen_state = END;
            global.draw = (global.amount_checked == 0); // stalemate : checkmate
        }
    }
    else {
        global.screen_state = ERROR;
    }
}

void move_king(int x, int y, e_piece type, uint64_t* moves)
{

    uint64_t heatmap = global.heatmap;

    board[y * GRID_X + x] = e_EMPTY;
    heat_map enemy = build_heatmap(global.turn == t_WHITE ? w_KING : b_KING);
    board[y * GRID_X + x] = type;

    int k_x = 0, k_y = 0;
    bool found = find_piece_position(&k_x, &k_y, global.turn == t_WHITE ? b_KING : w_KING);

    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            e_piece p = board_at(j, i);
            bool heat = (bool)get_bit(heatmap, j, i);
            if (!heat && (is_empty(p) || is_enemy(type, p))) {
                bool slider = (bool)get_bit(enemy.heatmap, j, i);
                if (!slider) {
                    set_bit(*moves, j, i);
                }
            }
        }
    }

    for (int i = k_y - 1; i <= k_y + 1; i++) {
        for (int j = k_x - 1; j <= k_x + 1; j++) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            if (i == k_y && j == k_x) continue;
            
            bool our_king_move = (bool)get_bit(*moves, j, i);
            if (our_king_move == 1) {
                unset_bit(*moves, j, i);
            }
        }
    }

    if (global.amount_checked == 0 && ((type == w_KING && y == 7) || (type == b_KING && y == 0))) {
        // king side castle
        if (can_castle_right(x, y, type)) {
            set_bit(*moves, (x + 2), y);
            global.castling = true;
        }
        if (can_castle_left(x, y, type)) {
            set_bit(*moves, (x - 2), y);
            global.castling = true;
        }
    }
}
bool can_castle_right(int x, int y, e_piece type)
{
    return board_at(x + 1, y) == e_EMPTY && 
           board_at(x + 2, y) == e_EMPTY &&
            (bool)get_bit(global.heatmap, (x + 1), y) == 0 &&
            (bool)get_bit(global.heatmap, (x + 2), y) == 0 &&
           ((type == w_KING && board_at(x + 3, y) == w_ROOK) || (type == b_KING && board_at(x + 3, y) == b_ROOK));
}

bool can_castle_left(int x, int y, e_piece type)
{
    return board_at(x - 1, y) == e_EMPTY && 
           board_at(x - 2, y) == e_EMPTY && 
           board_at(x - 3, y) == e_EMPTY &&
            (bool)get_bit(global.heatmap, (x - 1), y) == 0 &&
            (bool)get_bit(global.heatmap, (x - 2), y) == 0 &&
           ((type == w_KING && board_at(x - 4, y) == w_ROOK) || (type == b_KING && board_at(x - 4, y) == b_ROOK));
}

bool is_digit(char c)
{
    return c>= '0' && c<= '9';
}
void load_board(char* str)
{
    // fen notation
    char c;
    size_t i = 0;
    while ((c = *(str)++)) {
        if (i == GRID_SIZE) break;
        if (is_digit(c)) {
            size_t skip = c - '0';
            while (skip-- > 0) {
                board[i] = e_EMPTY;
                i++;
            }
            continue;
        }
        if (c == ' ' || c == '/') continue;
        board[i] = char_to_enum(c);
        i++;
    }
    set_turn(*str);
    assert(i == GRID_SIZE && "Wrong amount of pieces..");
}

void set_turn(char c)
{

    switch (c) {
        case 'b':
            global.turn = true;
        break;
        case 'w':
            global.turn = false;
        break;
        default:
            global.turn = false;
        break;
    }
}

e_piece char_to_enum(char c)
{
    switch (c) {
        case 'p':
            return b_PAWN;
        case 'n':
            return b_KNIGHT;
        case 'b':
            return b_BISHOP;
        case 'r':
            return b_ROOK;
        case 'q':
            return b_QUEEN;
        case 'k':
            return b_KING;

        case 'P':
            return w_PAWN;
        case 'N':
            return w_KNIGHT;
        case 'B':
            return w_BISHOP;
        case 'R':
            return w_ROOK;
        case 'Q':
            return w_QUEEN;
        case 'K':
            return w_KING;
        default:
            return e_EMPTY;
    }
}
char enum_to_char(e_piece p)
{
    switch (p) {
        case b_PAWN:
            return 'p';
        case b_KNIGHT:
            return 'n';
        case b_BISHOP:
            return 'b';
        case b_ROOK:
            return 'r';
        case b_QUEEN:
            return 'q';
        case b_KING:
            return 'k';

        case w_PAWN:
            return 'P';
        case w_KNIGHT:
            return 'N';
        case w_BISHOP: 
            return 'B';
        case w_ROOK:
            return 'R';
        case w_QUEEN:
            return 'Q';
        case w_KING: 
            return 'K';
        default:
            return ' ';

    }
}

void print_board(uint64_t x) {
    puts("--------------");
    for (uint64_t i = 0; i < 8; i++) {
        for (uint64_t j = 0; j < 8; j++) {
            uint64_t shift_amount = (63 - (i * 8 + j));
            printf("%ld ", ((x >> shift_amount) & 1UL));
        }
        printf("\n");
    }
}

e_piece board_at(int x, int y)
{
    return board[y * GRID_X + x];
}

bool is_white(e_piece p)
{ 
    return p > 0;
}
bool is_empty(e_piece p)
{ 
    return p == 0;
}
bool is_black(e_piece p)
{ 
    return p < 0; 
}
bool in_single_check(e_piece p)
{ 
    return global.amount_checked == 1 && ((is_white(p) && global.checked == c_WHITE) || (is_black(p) && global.checked == c_BLACK)); 
}
bool is_enemy(e_piece a, e_piece b)
{
    return (is_white(a) && is_black(b)) || (is_black(a) && is_white(b));
}

bool is_slider(e_piece a)
{
    return (a >= 4 && a <= 9) || (a <= -4 && a >= -9);
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

bool in_single_check_by_slider(e_piece p)
{
    return (in_single_check(p) && is_slider(board_at(global.checking_x, global.checking_y))) || in_single_check(p);
}


bool is_checked(e_piece type)
{
    return ((type == b_KING && global.checked == c_BLACK) || (type == w_KING && global.checked == c_WHITE)) && global.amount_checked > 0;
}

void copy_board(e_piece* dst, e_piece* src)
{
    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            dst[i * GRID_X + j] = src[i * GRID_X + j];
        }
    }
}

