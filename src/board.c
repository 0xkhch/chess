#include "board.h"
#include "ctype.h"
#include "piece.h"
#include "stdlib.h"

e_piece board[BOARD_SIZE] = {0};


void move_pawn(int x,   int y, e_piece type, uint64_t* moves);
void move_knight(int x, int y, e_piece type, uint64_t* moves);
void move_bishop(int x, int y, e_piece type, uint64_t* moves);
void move_rook(int x,   int y, e_piece type, uint64_t* moves);
void move_king(int x,   int y, e_piece type, uint64_t* moves);
static inline bool is_white(e_piece p);
static inline bool is_empty(e_piece p);
static inline bool is_black(e_piece p);
static inline bool is_enemy(e_piece a, e_piece b);

void find_selected_moves(global_state_t* global)
{
    global->selected_moves = 0;
    switch (global->selected_type) {
        case w_PAWN:
        case b_PAWN:
            move_pawn(global->selected_x, global->selected_y, global->selected_type, &global->selected_moves);
        break;
        case w_KNIGHT:
        case b_KNIGHT:
            move_knight(global->selected_x, global->selected_y, global->selected_type, &global->selected_moves);
        break;
        case w_BISHOP: 
        case b_BISHOP:
            move_bishop(global->selected_x, global->selected_y, global->selected_type, &global->selected_moves);
        break;
        case w_ROOK:
        case b_ROOK:
            move_rook(global->selected_x, global->selected_y, global->selected_type, &global->selected_moves);
        break;
        case w_QUEEN:
        case b_QUEEN:
            move_rook(global->selected_x, global->selected_y, global->selected_type, &global->selected_moves);
            move_bishop(global->selected_x, global->selected_y, global->selected_type, &global->selected_moves);
        break;

        case w_KING: 
        case b_KING:
            move_king(global->selected_x, global->selected_y, global->selected_type, &global->selected_moves);
        break;

        default:
        break;
    }
}


void move_pawn(int x, int y, e_piece type, uint64_t* moves)
{
    int pos = y * GRID_Y + x;
    if (type == w_PAWN) {
        if (is_empty(board[pos - GRID_X])) {
            set_bit(*moves, x, (y - 1));
            if (is_empty(board[pos - (GRID_X*2)]) && y == 6) {
                set_bit(*moves, x, (y - 2));
            }
        }
        if (is_black(board[pos - GRID_X + 1]) && x != GRID_X - 1) {
            set_bit(*moves, (x + 1), (y - 1));
        }
        if (is_black(board[pos - GRID_X - 1]) && x != 0) {
            set_bit(*moves, (x - 1), (y - 1));
        }
    }
    else if (type == b_PAWN) {
        if (pos >= GRID_X && pos < GRID_SIZE - GRID_X) {
            if (is_empty(board[pos + GRID_X])) {
                set_bit(*moves, x, (y + 1));
                if (is_empty(board[pos + (GRID_X*2)]) && y == 1) {
                    set_bit(*moves, x, (y + 2));
                }
            }
            if (is_white(board[pos + GRID_X + 1]) && x != GRID_X - 1) {
                set_bit(*moves, (x + 1), (y + 1));
            }
            if (is_white(board[pos + GRID_X - 1]) && x != 0) {
                set_bit(*moves, (x - 1), (y + 1));
            }
        }
    }
}

void move_knight(int x, int y, e_piece type, uint64_t* moves)
{
    // wide
    for (int i = y - 1; i <= y + 1; i = i + 2) {
        for (int j = x - 2; j <= x + 2; j = j + 4) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            e_piece p = board[i * GRID_Y + j];
            if (is_empty(p) || is_enemy(type, p)) {
                set_bit(*moves, j, i);
            }
        }
    }
    // long 
    for (int i = y - 2; i <= y + 2; i = i + 4) {
        for (int j = x - 1; j <= x + 1; j = j + 2) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            e_piece p = board[i * GRID_Y + j];
            if (is_empty(p) || is_enemy(type, p)) {
                set_bit(*moves, j, i);
            }
        }
    }
}

void move_axis(int x, int y, int dir_x, int dir_y, e_piece type, uint64_t* moves)
{
    int dx = x + dir_x;
    int dy = y + dir_y;
    while (dx >= 0 && dx < GRID_X && dy >= 0 && dy < GRID_Y) {
        e_piece p = board[dy * GRID_Y + dx];
        if (is_empty(p)) {
            set_bit(*moves, dx, dy);
        }
        else {
            if (is_enemy(type, p)) {
                set_bit(*moves, dx, dy);
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

void move_king(int x, int y, e_piece type, uint64_t* moves)
{ 
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            e_piece p = board[i * GRID_Y + j];
            if (is_empty(p) || is_enemy(type, p)) {
                set_bit(*moves, j, i);
            }
        }
    }
}

void init_board(char* str)
{
    // fen notation
    char c;
    size_t i = 0;
    while ((c = *(str)++)) {
        if (isdigit(c)) {
            size_t skip = c - '0';
            while (skip-- > 0) {
                board[i] = e_EMPTY;
                i++;
            }
            continue;
        }
        switch (c) {
            case '/':
                continue;
            break;
            case ' ':
                continue;
            case 'p':
                board[i] = b_PAWN;
            break;
            case 'n':
                board[i] = b_KNIGHT;
            break;
            case 'b':
                board[i] = b_BISHOP;
            break;
            case 'r':
                board[i] = b_ROOK;
            break;
            case 'q':
                board[i] = b_QUEEN;
            break;
            case 'k':
                board[i] = b_KING;
            break;

            case 'P':
                board[i] = w_PAWN;
            break;
            case 'N':
                board[i] = w_KNIGHT;
            break;
            case 'B':
                board[i] = w_BISHOP;
            break;
            case 'R':
                board[i] = w_ROOK;
            break;
            case 'Q':
                board[i] = w_QUEEN;
            break;
            case 'K':
                board[i] = w_KING;
            break;
        }
        i++;
    }
    assert(i == GRID_X * GRID_Y && "Wrong amount of pieces..");
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

static inline bool is_white(e_piece p)
{ 
    return p > 0;
}
static inline bool is_empty(e_piece p)
{ 
    return p == 0;
}
static inline bool is_black(e_piece p)
{ 
    return p < 0; 
}
static inline bool is_enemy(e_piece a, e_piece b)
{
    return (is_white(a) && is_black(b)) || (is_black(a) && is_white(b));
}
