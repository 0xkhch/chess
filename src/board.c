#include "board.h"
#include "ctype.h"
#include "piece.h"
#include "stdlib.h"

e_piece board[GRID_X * GRID_Y] = {0};
bool possible_moves[GRID_X * GRID_Y] = {0};
e_turn turn = e_WHITE;

void reset_possible_moves(void)
{
    for (size_t i = 0; i < GRID_Y * GRID_X; i++) {
        possible_moves[i] = false;
    }
}
int find_possible_moves(int x, int y, e_piece type)
{
    int num = 0; // number of avaliable places
    int pos = y * GRID_Y + x;
    switch (type) {
        case e_EMPTY: {
            reset_possible_moves();
        } break;
        case w_PAWN: {
            if (pos >= GRID_X && pos < GRID_SIZE) {
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
        } break;
        case w_KNIGHT: {
            if (pos >= 0 && pos < GRID_SIZE) {
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
        } break;
        case w_BISHOP: {
                for (int i = y - 1; i >= 0; i--) {
                    if ((x + (y - i)) >= GRID_X) break; // if the x position goes past the grid
                    num++;
                    int curr = i * GRID_Y + (x + (y - i));
                    possible_moves[curr] = board[curr] <= 0 ? true : false;
                    if (board[curr] != 0) break;
                }

                for (int i = y - 1; i >= 0; i--) {
                    if ((x - (y - i)) < 0) break;
                    num++;
                    int curr = i * GRID_Y + (x - (y - i));
                    possible_moves[curr] = board[curr] <= 0 ? true : false;
                    if (board[curr] != 0) break;
                }

                for (int i = y + 1; i < GRID_Y; i++) {
                    if ((x + (i - y)) >= GRID_X) break;
                    num++;
                    int curr = i * GRID_Y + (x + (i - y));
                    possible_moves[curr] = board[curr] <= 0 ? true : false;
                    if (board[curr] != 0) break;
                }

                for (int i = y + 1; i < GRID_Y; i++) {
                    if ((x - (i - y)) < 0) break;
                    num++;
                    int curr = i * GRID_Y + (x - (i - y));
                    possible_moves[curr] = board[curr] <= 0 ? true : false;
                    if (board[curr] != 0) break;
                }
        } break;
        case w_ROOK: {
                for (int i = y - 1; i >= 0; i--) {
                    num++;
                    possible_moves[i * GRID_Y + x] = board[i * GRID_Y + x] <= 0 ? true : false;
                    if (board[i * GRID_Y + x] != 0) break;
                }
                for (int i = x + 1; i < GRID_X; i++) {
                    num++;
                    possible_moves[y * GRID_Y + i] = board[y * GRID_Y + i] <= 0 ? true : false;
                    if (board[y * GRID_Y + i] != 0) break;
                }

                for (int i = y + 1; i < GRID_Y; i++) {
                    num++;
                    possible_moves[i * GRID_Y + x] = board[i * GRID_Y + x] <= 0 ? true : false;
                    if (board[i * GRID_Y + x] != 0) break;
                }

                for (int i = x - 1; i >= 0; i--) {
                    num++;
                    possible_moves[y * GRID_Y + i] = board[y * GRID_Y + i] <= 0 ? true : false;
                    if (board[y * GRID_Y + i] != 0) break;
                }
        } break;
        case w_QUEEN: {
        } break;
        case w_KING: {
        } break;

        case b_PAWN: {
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
        } break;
        case b_KNIGHT: {
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
        } break;
        case b_BISHOP: {
                for (int i = y - 1; i >= 0; i--) {
                    if ((x + (y - i)) >= GRID_X) break; // if the x position goes past the grid
                    num++;
                    int curr = i * GRID_Y + (x + (y - i));
                    possible_moves[curr] = board[curr] >= 0 ? true : false;
                    if (board[curr] != 0) break;
                }

                for (int i = y - 1; i >= 0; i--) {
                    if ((x - (y - i)) < 0) break;
                    num++;
                    int curr = i * GRID_Y + (x - (y - i));
                    possible_moves[curr] = board[curr] >= 0 ? true : false;
                    if (board[curr] != 0) break;
                }

                for (int i = y + 1; i < GRID_Y; i++) {
                    if ((x + (i - y)) >= GRID_X) break;
                    num++;
                    int curr = i * GRID_Y + (x + (i - y));
                    possible_moves[curr] = board[curr] >= 0 ? true : false;
                    if (board[curr] != 0) break;
                }

                for (int i = y + 1; i < GRID_Y; i++) {
                    if ((x - (i - y)) < 0) break;
                    num++;
                    int curr = i * GRID_Y + (x - (i - y));
                    possible_moves[curr] = board[curr] >= 0 ? true : false;
                    if (board[curr] != 0) break;
                }
        } break;
        case b_ROOK: {
            if (pos >= 0 && pos < GRID_SIZE) {
                for (int i = y - 1; i >= 0; i--) {
                    num++;
                    possible_moves[i * GRID_Y + x] = board[i * GRID_Y + x] >= 0 ? true : false;
                    if (board[i * GRID_Y + x] != 0) break;
                }
                for (int i = x + 1; i < GRID_X; i++) {
                    num++;
                    possible_moves[y * GRID_Y + i] = board[y * GRID_Y + i] >= 0 ? true : false;
                    if (board[y * GRID_Y + i] != 0) break;
                }

                for (int i = y + 1; i < GRID_Y; i++) {
                    num++;
                    possible_moves[i * GRID_Y + x] = board[i * GRID_Y + x] >= 0 ? true : false;
                    if (board[i * GRID_Y + x] != 0) break;
                }

                for (int i = x - 1; i >= 0; i--) {
                    num++;
                    possible_moves[y * GRID_Y + i] = board[y * GRID_Y + i] >= 0 ? true : false;
                    if (board[y * GRID_Y + i] != 0) break;
                }
            }
        } break;
        case b_QUEEN: {
        } break;
        case b_KING: {
        } break;
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
}
