#include "board.h"
#include "ctype.h"
#include "piece.h"
#include "stdlib.h"

e_piece board[BOARD_SIZE] = {0};

uint64_t possible_moves = 0;
uint64_t white_heatmap = 0;
uint64_t black_heatmap = 0;
bool white_heatmap[BOARD_SIZE] = {0};
bool black_heatmap[BOARD_SIZE] = {0};

uint64_t w_KNIGHT_moves = 0;

int move_pawn(int x, int y, e_piece type);
int move_knight(int x, int y, e_piece type);
int move_bishop(int x, int y, e_piece type);
int move_rook(int x, int y, e_piece type);
int move_king(int x, int y, e_piece type);

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

void reset_possible_moves(void)
{
    possible_moves = 0;
}


void reset_heatmap(bool type)
{
    if (type) {
    }
    else {
        for (size_t i = 0; i < GRID_SIZE; i++) {
            white_heatmap[i] = false;
        }
    }
}
int find_possible_moves(int x, int y, e_piece type)
{
    int num = 0; // number of avaliable places
    switch (type) {
        case e_EMPTY: {
            reset_possible_moves();
        } break;
        case w_PAWN:
        case b_PAWN: {
            num = move_pawn(x, y, type);
        } break;
        case w_KNIGHT:
        case b_KNIGHT: {
            num = move_knight(x, y, type);
        } break;
        case w_BISHOP: 
        case b_BISHOP: {
            num = move_bishop(x, y, type);
        } break;
        case w_ROOK:
        case b_ROOK: {
            num = move_rook(x, y, type);
        } break;
        case w_QUEEN: {
            num  = move_rook(x, y, w_ROOK);
            num += move_bishop(x, y, w_BISHOP);
            num += move_king(x, y, w_KING);
        } break;
        case b_QUEEN: {
            num  = move_rook(x, y, b_ROOK);
            num += move_bishop(x, y, b_BISHOP);
            num += move_king(x, y, b_KING);
        } break;

        case w_KING: 
        case b_KING: {
            num = move_king(x, y, type);
        } break;
    }
    return num;
}

int move_pawn(int x, int y, e_piece type)
{
    int num = 0;
    int pos = y * GRID_Y + x;
    if (pos < 0 || pos >= BOARD_SIZE || pos - GRID_X < 0 || pos + GRID_X >= BOARD_SIZE) return 0;
    if (type == w_PAWN) {
        if (board[pos - GRID_X] == 0) {
            num++;
            set_bit(possible_moves, x, (y - 1));
            if (board[pos - (GRID_X*2)] == 0 && y == 6) {
                set_bit(possible_moves, x, (y - 2));
                num++;
            }
        }
        if (board[pos - GRID_X + 1] < 0 && x != GRID_X - 1) {
            set_bit(possible_moves, (x + 1), (y - 1));
            num++;
        }
        if (board[pos - GRID_X - 1] < 0 && x != 0) {
            set_bit(possible_moves, (x - 1), (y - 1));
            num++;
        }
    }
    else if (type == b_PAWN) {
        if (pos >= GRID_X && pos < GRID_SIZE - GRID_X) {
            if (board[pos + GRID_X] == 0) {
                set_bit(possible_moves, x, (y + 1));
                num++;
                if (board[pos + (GRID_X*2)] == 0 && y == 1) {
                    set_bit(possible_moves, x, (y + 2));
                    num++;
                }
            }
            if (board[pos + GRID_X + 1] > 0 && x != GRID_X - 1) {
                num++;
                set_bit(possible_moves, (x + 1), (y + 1));
            }
            if (board[pos + GRID_X - 1] > 0 && x != 0) {
                num++;
                set_bit(possible_moves, (x - 1), (y + 1));
            }
        }
    }
    return num;
}

int move_knight(int x, int y, e_piece type)
{
    int num = 0;
    // wide
    for (int i = y - 1; i <= y + 1; i = i + 2) {
        for (int j = x - 2; j <= x + 2; j = j + 4) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            if (type == w_KNIGHT && board[i * GRID_Y + j] <= 0) {
                num++;
                set_bit(possible_moves, j, i);
            }
            if (type == b_KNIGHT && board[i * GRID_Y + j] >= 0) {
                num++;
                set_bit(possible_moves, j, i);
            }
        }
    }
    // long 
    for (int i = y - 2; i <= y + 2; i = i + 4) {
        for (int j = x - 1; j <= x + 1; j = j + 2) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            if (type == w_KNIGHT && board[i * GRID_Y + j] <= 0) {
                num++;
                set_bit(possible_moves, j, i);
            }
            if (type == b_KNIGHT && board[i * GRID_Y + j] >= 0) {
                num++;
                set_bit(possible_moves, j, i);
            }
        }
    }
    return num;
}

int move_bishop(int x, int y, e_piece type)
{
    //TODO: rewrite this i dont even understand this anymore dude
    int num = 0;
    typedef struct {
        int64_t x;
        int64_t y;
    } point;
    bool b_NE = false, b_SE = false, b_SW = false, b_NW = false;
    for (int64_t i = 1; i < GRID_Y; i++) { 

        point NE = {.x = x + i, .y = y - i};
        if (!b_NE && NE.x < GRID_X && NE.y >= 0) {
            if (board[NE.y * GRID_Y + NE.x] == 0) {
                num++;
                set_bit(possible_moves, NE.x, NE.y);
            }
            else if (type == w_BISHOP && board[NE.y * GRID_Y + NE.x] < 0) {
                num++;
                set_bit(possible_moves, NE.x, NE.y);
                b_NE = true;
            }
            else if (type == b_BISHOP && board[NE.y * GRID_Y + NE.x] > 0) {
                num++;
                set_bit(possible_moves, NE.x, NE.y);
                b_NE = true;
            }
            else {
                b_NE = true;
            }
        }

        point SE = {.x = x + i, .y = y + i};
        if (!b_SE && SE.x < GRID_X && SE.y < GRID_Y) {
            if (board[SE.y * GRID_Y + SE.x] == 0) {
                num++;
                set_bit(possible_moves, SE.x, SE.y);
            }
            else if (type == w_BISHOP && board[SE.y * GRID_Y + SE.x] < 0) {
                num++;
                set_bit(possible_moves, SE.x, SE.y);
                b_SE = true;
            }
            else if (type == b_BISHOP && board[SE.y * GRID_Y + SE.x] > 0) {
                num++;
                set_bit(possible_moves, SE.x, SE.y); b_SE = true;
            }
            else {
                b_SE = true;
            }
        }

        point SW = {.x = x - i, .y = y + i};
        if (!b_SW && SW.x >= 0 && SW.y < GRID_Y) {
            if (board[SW.y * GRID_Y + SW.x] == 0) {
                num++;
                set_bit(possible_moves, SW.x, SW.y);
            }
            else if (type == w_BISHOP && board[SW.y * GRID_Y + SW.x] < 0) {
                num++;
                set_bit(possible_moves, SW.x, SW.y);
                b_SW = true;
            }
            else if (type == b_BISHOP && board[SW.y * GRID_Y + SW.x] > 0) {
                num++;
                set_bit(possible_moves, SW.x, SW.y);
                b_SW = true;
            }
            else {
                b_SW = true;
            }
        }

        point NW = {.x = x - i, .y = y - i};
        if (!b_NW && NW.x >= 0 && NW.y >= 0) {
            if (board[NW.y * GRID_Y + NW.x] == 0) {
                num++;
                set_bit(possible_moves, NW.x, NW.y);
            }
            else if (type == w_BISHOP && board[NW.y * GRID_Y + NW.x] < 0) {
                num++;
                set_bit(possible_moves, NW.x, NW.y);
                b_NW = true;
            }
            else if (type == b_BISHOP && board[NW.y * GRID_Y + NW.x] > 0) {
                num++;
                set_bit(possible_moves, NW.x, NW.y);
                b_NW = true;
            }
            else {
                b_NW = true;
            }
        }
    }
    return num;
}


int move_rook(int x, int y, e_piece type)
{
    int num = 0;
    bool b_up = false, b_right = false, b_down = false, b_left = false;
    for (int64_t i = 1; i < GRID_Y; i++) {
        int64_t up = y - i;
        if (!b_up && up >= 0) {
            if (board[up * GRID_Y + x] == 0) {
                num++;
                set_bit(possible_moves, x, up);
            }
            else if (type == w_ROOK && board[up * GRID_Y + x] < 0) {
                num++;
                set_bit(possible_moves, x, up);
                b_up = true;
            }
            else if (type == b_ROOK && board[up * GRID_Y + x] > 0) {
                num++;
                set_bit(possible_moves, x, up);
                b_up = true;
            }
            else {
                b_up = true;
            }
        }

        int64_t right = x + i;
        if (!b_right && right < GRID_X) {
            if (board[y * GRID_Y + right] == 0) {
                num++;
                set_bit(possible_moves, right, y);
            }
            else if (type == w_ROOK && board[y * GRID_Y + right] < 0) {
                num++;
                set_bit(possible_moves, right, y);
                b_right = true;
            }
            else if (type == b_ROOK && board[y * GRID_Y + right] > 0) {
                num++;
                set_bit(possible_moves, right, y);
                b_right = true;
            }
            else {
                b_right = true;
            }
        }
        int64_t down = y + i;
        if (!b_down && down < GRID_Y) {
            if (board[down * GRID_Y + x] == 0) {
                num++;
                set_bit(possible_moves, x, down);
            }
            else if (type == w_ROOK && board[y * GRID_Y + x] < 0) {
                num++;
                set_bit(possible_moves, x, down);
                b_down = true;
            }
            else if (type == b_ROOK && board[y * GRID_Y + x] > 0) {
                num++;
                set_bit(possible_moves, x, down);
                b_down = true;
            }
            else {
                b_down = true;
            }
        }
        int64_t left = x - i;
        if (!b_left && left >= 0) {
            if (board[y * GRID_Y + left] == 0) {
                num++;
                set_bit(possible_moves, left, y);
            }
            else if (type == w_ROOK && board[y * GRID_Y + left] < 0) {
                num++;
                set_bit(possible_moves, left, y);
                b_left = true;
            }
            else if (type == b_ROOK && board[y * GRID_Y + left] > 0) {
                num++;
                set_bit(possible_moves, left, y);
                b_left = true;
            }
            else {
                b_left = true;
            }
        }
    }
    return num;
}

int move_king(int x, int y, e_piece type)
{
    int num = 0;
    // puts("==============");
    // printf("y = %d\n", i);
    // printf("    x = %d\n", j);
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (j >= GRID_X || i >= GRID_Y || j < 0 || i < 0) continue;
            if (type == w_KING && board[i * GRID_Y + j] <= 0) {
                num++;
                set_bit(possible_moves, j, i);
            }
            if (type == b_KING && board[i * GRID_Y + j] >= 0) {
                num++;
                set_bit(possible_moves, j, i);
            }
        }
    }
    return num;
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
