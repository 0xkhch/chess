#include "board.h"
#include "ctype.h"
#include "piece.h"
#include "stdlib.h"

e_piece board[GRID_X * GRID_Y] = {0};
bool possible_moves[GRID_X * GRID_Y] = {0};
e_turn turn = e_WHITE;

int move_pawn(int x, int y, e_piece type);
int move_knight(int x, int y, e_piece type);
int move_bishop(int x, int y, e_piece type);
int move_rook(int x, int y, e_piece type);

void reset_possible_moves(void)
{
    for (size_t i = 0; i < GRID_Y * GRID_X; i++) {
        possible_moves[i] = false;
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
        case w_QUEEN:
        case b_QUEEN: {
        } break;

        case w_KING: {
        } break;
        case b_KING: {
        } break;
    }
    return num;
}

int move_pawn(int x, int y, e_piece type)
{
    int num = 0;
    int pos = y * GRID_Y + x;
    if (pos >= GRID_X && pos < GRID_SIZE) {
        if (type == w_PAWN) {
            if (board[pos - GRID_X] == 0) {
                num++;
                possible_moves[pos - GRID_X] = true;
                if (board[pos - (GRID_X*2)] == 0 && y == 6) {
                    possible_moves[pos - (GRID_X*2)] = true;
                    num++;
                }
            }
            if (board[pos - GRID_X + 1] < 0 && x != GRID_X - 1) {
                possible_moves[pos - GRID_X + 1] = true;
                num++;
            }
            if (board[pos - GRID_X - 1] < 0) {
                possible_moves[pos - GRID_X - 1] = true;
                num++;
            }
        }
        else if (type == b_PAWN) {
            if (pos >= GRID_X && pos < GRID_SIZE - GRID_X) {
                if (board[pos + GRID_X] == 0) {
                    possible_moves[pos + GRID_X] = true;
                    num++;
                    if (board[pos + (GRID_X*2)] == 0 && y == 1) {
                        possible_moves[pos + (GRID_X*2)] = true;
                        num++;
                    }
                }
                if (board[pos + GRID_X + 1] > 0) {
                    num++;
                    possible_moves[pos + GRID_X + 1] = true;
                }
                if (board[pos + GRID_X - 1] > 0 && x != 0) {
                    num++;
                    possible_moves[pos + GRID_X - 1] = true;
                }
            }
        }
    }
    return num;
}

int move_knight(int x, int y, e_piece type)
{
    int num = 0;
    int pos = y * GRID_Y + x;
    if (pos >= 0 && pos < GRID_SIZE) {
        if (type ==  w_KNIGHT) {
            if (x != GRID_X - 1 && pos - (GRID_Y*2) + 1 >= 0 && board[pos - (GRID_Y*2) + 1] <= 0) {
                num++;
                possible_moves[pos - (GRID_Y*2) + 1] = true; // top right
            }
            if (x != 0 && pos - (GRID_Y*2) - 1 >= 0 && board[pos - (GRID_Y*2) - 1] <= 0) {
                num++;
                possible_moves[pos - (GRID_Y*2) - 1] = true; // top left
            }

            if (x < GRID_X - 2 && pos - (GRID_Y) + 2 >= 0 && board[pos - (GRID_Y) + 2] <= 0) {
                num++;
                possible_moves[pos - (GRID_Y) + 2] = true; // upper right
            }
            if (x > 1 && pos - (GRID_Y) - 2 >= 0 && board[pos - (GRID_Y) - 2] <= 0) {
                num++;
                possible_moves[pos - (GRID_Y) - 2] = true; // upper left
            }

            if (x != GRID_X - 1 && pos + (GRID_Y*2) + 1 < GRID_SIZE && board[pos + (GRID_Y*2) + 1] <= 0) {
                num++;
                possible_moves[pos + (GRID_Y*2) + 1] = true; // bottom right
            }
            if (x != 0 && pos + (GRID_Y*2) - 1 < GRID_SIZE && board[pos + (GRID_Y*2) - 1] <= 0) {
                num++;
                possible_moves[pos + (GRID_Y*2) - 1] = true; // bottom left
            }

            if (x < GRID_X - 2 && pos + (GRID_Y) + 2 < GRID_SIZE && board[pos + (GRID_Y) + 2] <= 0) {
                num++;
                possible_moves[pos + (GRID_Y) + 2] = true; // lower right
            }
            if (x > 1 && pos + (GRID_Y) - 2 < GRID_SIZE && board[pos + (GRID_Y) - 2] <= 0) {
                num++;
                possible_moves[pos + (GRID_Y) - 2] = true; // lower left
            }
        }
        else if (type == b_PAWN) {
            if (pos >= 0 && pos < GRID_SIZE) {
                if (x != GRID_X - 1 && pos - (GRID_Y*2) + 1 >= 0 && board[pos - (GRID_Y*2) + 1] >= 0) {
                    num++;
                    possible_moves[pos - (GRID_Y*2) + 1] = true; // top right
                }
                if (x != 0 && pos - (GRID_Y*2) - 1 >= 0 && board[pos - (GRID_Y*2) - 1] >= 0) {
                    num++;
                    possible_moves[pos - (GRID_Y*2) - 1] = true; // top left
                }

                if (x < GRID_X - 2 && pos - (GRID_Y) + 2 >= 0 && board[pos - (GRID_Y) + 2] >= 0) {
                    num++;
                    possible_moves[pos - (GRID_Y) + 2] = true; // upper right
                }
                if (x > 1 && pos - (GRID_Y) - 2 >= 0 && board[pos - (GRID_Y) - 2] >= 0) {
                    num++;
                    possible_moves[pos - (GRID_Y) - 2] = true; // upper left
                }

                if (x != GRID_X - 1 && pos + (GRID_Y*2) + 1 < GRID_SIZE && board[pos + (GRID_Y*2) + 1] >= 0) {
                    num++;
                    possible_moves[pos + (GRID_Y*2) + 1] = true; // bottom right

                }
                if (x != 0 && pos + (GRID_Y*2) - 1 < GRID_SIZE && board[pos + (GRID_Y*2) - 1] >= 0) {
                    num++;
                    possible_moves[pos + (GRID_Y*2) - 1] = true; // bottom left
                }

                if (x < GRID_X - 2 && pos + (GRID_Y) + 2 < GRID_SIZE && board[pos + (GRID_Y) + 2] >= 0) {
                    num++;
                    possible_moves[pos + (GRID_Y) + 2] = true; // lower right
                }
                if (x > 1 && pos + (GRID_Y) - 2 < GRID_SIZE && board[pos + (GRID_Y) - 2] >= 0) {
                    num++;
                    possible_moves[pos + (GRID_Y) - 2] = true; // lower left
                }
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
            possible_moves[curr] = board[curr] <= 0 ? true : false;
        }
        else if (type == b_BISHOP) {
            possible_moves[curr] = board[curr] >= 0 ? true : false;
        }
        if (board[curr] != 0) break;
    }

    for (int i = y - 1; i >= 0; i--) {
        if ((x - (y - i)) < 0) break;
        num++;
        int curr = i * GRID_Y + (x - (y - i));
        if (type == w_BISHOP) {
            possible_moves[curr] = board[curr] <= 0 ? true : false;
        }
        else if (type == b_BISHOP) {
            possible_moves[curr] = board[curr] >= 0 ? true : false;
        }
        if (board[curr] != 0) break;
    }

    for (int i = y + 1; i < GRID_Y; i++) {
        if ((x + (i - y)) >= GRID_X) break;
        num++;
        int curr = i * GRID_Y + (x + (i - y));
        if (type == w_BISHOP) {
            possible_moves[curr] = board[curr] <= 0 ? true : false;
        }
        else if (type == b_BISHOP) {
            possible_moves[curr] = board[curr] >= 0 ? true : false;
        }
        if (board[curr] != 0) break;
    }

    for (int i = y + 1; i < GRID_Y; i++) {
        if ((x - (i - y)) < 0) break;
        num++;
        int curr = i * GRID_Y + (x - (i - y));
        if (type == w_BISHOP) {
            possible_moves[curr] = board[curr] <= 0 ? true : false;
        }
        else if (type == b_BISHOP) {
            possible_moves[curr] = board[curr] >= 0 ? true : false;
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
            possible_moves[i * GRID_Y + x] = board[i * GRID_Y + x] <= 0 ? true : false;
        }
        else if (type == b_ROOK) {
            possible_moves[i * GRID_Y + x] = board[i * GRID_Y + x] >= 0 ? true : false;
        }
        if (board[i * GRID_Y + x] != 0) break;
    }
    for (int i = x + 1; i < GRID_X; i++) {
        num++;
        if (type == w_ROOK) {
            possible_moves[i * GRID_Y + x] = board[i * GRID_Y + x] <= 0 ? true : false;
        }
        else if (type == b_ROOK) {
            possible_moves[i * GRID_Y + x] = board[i * GRID_Y + x] >= 0 ? true : false;
        }
        if (board[y * GRID_Y + i] != 0) break;
    }

    for (int i = y + 1; i < GRID_Y; i++) {
        num++;
        if (type == w_ROOK) {
            possible_moves[i * GRID_Y + x] = board[i * GRID_Y + x] <= 0 ? true : false;
        }
        else if (type == b_ROOK) {
            possible_moves[i * GRID_Y + x] = board[i * GRID_Y + x] >= 0 ? true : false;
        }
        if (board[i * GRID_Y + x] != 0) break;
    }

    for (int i = x - 1; i >= 0; i--) {
        num++;
        if (type == w_ROOK) {
            possible_moves[i * GRID_Y + x] = board[i * GRID_Y + x] <= 0 ? true : false;
        }
        else if (type == b_ROOK) {
            possible_moves[i * GRID_Y + x] = board[i * GRID_Y + x] >= 0 ? true : false;
        }
        if (board[y * GRID_Y + i] != 0) break;
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
