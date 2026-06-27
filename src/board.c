#include "board.h"
#include "ctype.h"
#include "piece.h"
#include "stdlib.h"

e_piece board[BOARD_SIZE] = {0};

uint64_t possible_moves = 0;
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
        print_board(possible_moves);
        exit(0);
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
                set_bit(possible_moves, (x + 1), (y + 2));
            }
            if (board[pos + GRID_X - 1] > 0 && x != 0) {
                num++;
                set_bit(possible_moves, (x - 1), (y + 2));
            }
        }
    }
    return num;
}

int move_knight(int x, int y, e_piece type)
{
    int num = 0;
    int pos = y * GRID_Y + x;
    if (pos < 0 || pos >= BOARD_SIZE || pos - GRID_X < 0 || pos + GRID_X >= BOARD_SIZE) return 0;
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
    int num = 0;
    for (int i = y - 1; i >= 0; i--) {
        if ((x + (y - i)) >= GRID_X) break; // if the x position goes past the grid
        num++;
        int curr = i * GRID_Y + (x + (y - i));
        if (type == w_BISHOP) {
            board[curr] <= 0 ? set_bit_offset(possible_moves, curr) : unset_bit_offset(possible_moves, curr);
        }
        else if (type == b_BISHOP) {
            board[curr] >= 0 ? set_bit_offset(possible_moves, curr) : unset_bit_offset(possible_moves, curr);
        }
        if (board[curr] != 0) break;
    }

    for (int i = y - 1; i >= 0; i--) {
        if ((x - (y - i)) < 0) break;
        num++;
        int curr = i * GRID_Y + (x - (y - i));
        if (type == w_BISHOP) {
            board[curr] <= 0 ? set_bit_offset(possible_moves, curr) : unset_bit_offset(possible_moves, curr);
        }
        else if (type == b_BISHOP) {
            board[curr] >= 0 ? set_bit_offset(possible_moves, curr) : unset_bit_offset(possible_moves, curr);
        }
        if (board[curr] != 0) break;
    }

    for (int i = y + 1; i < GRID_Y; i++) {
        if ((x + (i - y)) >= GRID_X) break;
        num++;
        int curr = i * GRID_Y + (x + (i - y));
        if (type == w_BISHOP) {
            board[curr] <= 0 ? set_bit_offset(possible_moves, curr) : unset_bit_offset(possible_moves, curr);
        }
        else if (type == b_BISHOP) {
            board[curr] >= 0 ? set_bit_offset(possible_moves, curr) : unset_bit_offset(possible_moves, curr);
        }
        if (board[curr] != 0) break;
    }

    for (int i = y + 1; i < GRID_Y; i++) {
        if ((x - (i - y)) < 0) break;
        num++;
        int curr = i * GRID_Y + (x - (i - y));
        if (type == w_BISHOP) {
            board[curr] <= 0 ? set_bit_offset(possible_moves, curr) : unset_bit_offset(possible_moves, curr);
        }
        else if (type == b_BISHOP) {
            board[curr] >= 0 ? set_bit_offset(possible_moves, curr) : unset_bit_offset(possible_moves, curr);
        }
        if (board[curr] != 0) break;
    }
    return num;
}


int move_rook(int x, int y, e_piece type)
{
    int num = 0;
    for (int i = y - 1; i >= 0; i--) {
        num++;
        if (type == w_ROOK) {
            board[i * GRID_Y + x] <= 0 ? set_bit(possible_moves, x, y): unset_bit(possible_moves, x, y);
        }
        else if (type == b_ROOK) {
            board[i * GRID_Y + x] >= 0 ? set_bit(possible_moves, x, y): unset_bit(possible_moves, x, y);
        }
        if (board[i * GRID_Y + x] != 0) break;
    }
    for (int i = x + 1; i < GRID_X; i++) {
        num++;
        if (type == w_ROOK) {
            board[i * GRID_Y + x] <= 0 ? set_bit(possible_moves, x, y): unset_bit(possible_moves, x, y);
        }
        else if (type == b_ROOK) {
            board[i * GRID_Y + x] >= 0 ? set_bit(possible_moves, x, y): unset_bit(possible_moves, x, y);
        }
        if (board[y * GRID_Y + i] != 0) break;
    }

    for (int i = y + 1; i < GRID_Y; i++) {
        num++;
        if (type == w_ROOK) {
            board[i * GRID_Y + x] <= 0 ? set_bit(possible_moves, x, y): unset_bit(possible_moves, x, y);
        }
        else if (type == b_ROOK) {
            board[i * GRID_Y + x] >= 0 ? set_bit(possible_moves, x, y): unset_bit(possible_moves, x, y);
        }
        if (board[i * GRID_Y + x] != 0) break;
    }

    for (int i = x - 1; i >= 0; i--) {
        num++;
        if (type == w_ROOK) {
            board[i * GRID_Y + x] <= 0 ? set_bit(possible_moves, x, y): unset_bit(possible_moves, x, y);
        }
        else if (type == b_ROOK) {
            board[i * GRID_Y + x] >= 0 ? set_bit(possible_moves, x, y): unset_bit(possible_moves, x, y);
        }
        if (board[y * GRID_Y + i] != 0) break;
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
