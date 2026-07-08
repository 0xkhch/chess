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
void find_moves(int x, int y, e_piece type, uint64_t* moves);
void print_board(uint64_t x);
e_piece char_to_enum(char c);
char enum_to_char(e_piece p);
void set_turn(char c);
bool is_enemy(e_piece a, e_piece b);
bool is_enemy_king(e_piece piece, bool turn);

void find_selected_moves() {
    global.selected_moves = 0;
    global.heatmap = 0;
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
            e_piece p = board_at(j, i);
            if (is_empty(p) || is_enemy(type, p)) {
                set_bit(*moves, j, i);
            }
        }
    }
    // long 
    for (int i = y - 2; i <= y + 2; i = i + 4) {
        for (int j = x - 1; j <= x + 1; j = j + 2) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            e_piece p = board_at(j, i);
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
        e_piece p = board_at(dx, dy);
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

void build_heatmap(e_piece type, uint64_t* moves)
{
    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            e_piece p = board_at(j, i);
            if (is_enemy(type, p)) {
                find_moves(j, i, p, moves);
            }
        }
    }
}

void check()
{
    uint64_t moves = 0;
    build_heatmap(global.turn == t_WHITE ? w_KING : b_KING, &moves);
    global.heatmap = moves;

    //TODO: possible cache the king positions so we dont have to search for it again...
    int x = 0;
    int y = 0;
    for (int i = 0; i <= GRID_Y; i++) {
        for (int j = 0; j <= GRID_X; j++) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            e_piece p = board_at(j, i);
            if (p == w_KING && global.turn == t_WHITE) {
                x = j;
                y = i;
            }
            if (p == b_KING && global.turn == t_BLACK) {
                x = j;
                y = i;
            }
        }
    }
    bool heat = (bool)get_bit(moves, x, y);
    global.check = heat ? true : false;
    global.checked = (heat && global.turn == t_WHITE) ? c_WHITE : c_BLACK;
}
uint64_t slider_heatmap(int x, int y, e_piece type)
{
    uint64_t moves = 0;
    //remove king
    board[y * GRID_Y + x] = e_EMPTY;
    build_heatmap(type, &moves);
    //restore king
    board[y * GRID_Y + x] = type;
    return moves;
}

void move_king(int x, int y, e_piece type, uint64_t* moves)
{ 
    uint64_t heatmap = slider_heatmap(x, y, type);
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            e_piece p = board_at(j, i);
            bool heat = (bool)get_bit(heatmap, j, i);
            if (!heat && (is_empty(p) || is_enemy(type, p))) {
                set_bit(*moves, j, i);
            }
        }
    }
}

void load_board(char* str)
{
    // fen notation
    char c;
    size_t i = 0;
    while ((c = *(str)++)) {
        if (i == GRID_SIZE) break;
        if (isdigit(c)) {
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
#ifdef DEBUG
    set_turn(*str);
#endif /* ifdef DEBUG */
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
    return board[y * GRID_Y + x];
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
bool is_enemy(e_piece a, e_piece b)
{
    return (is_white(a) && is_black(b)) || (is_black(a) && is_white(b));
}

bool is_enemy_king(e_piece piece, bool turn) {
    return (turn == t_WHITE && piece == b_KING) || (turn == t_BLACK && piece == w_KING);
}
