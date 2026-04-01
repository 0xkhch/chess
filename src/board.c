#include "board.h"
#include "ctype.h"
#include "stdlib.h"

e_piece board[GRID_X * GRID_Y] = {0};
bool possible_moves[GRID_X][GRID_Y] = {0};
e_turn turn = e_WHITE;

void reset_possible_moves(void)
{
    for (size_t i = 0; i < GRID_Y; i++) {
        for (size_t j = 0; j < GRID_X; j++) {
            possible_moves[i][j] = false;
        }
    }
}
void find_possible_moves(size_t x, size_t y, e_piece type)
{
    // TODO: fix this
    switch (type) {
        case e_EMPTY: {
            reset_possible_moves();
        } break;
    //     case w_PAWN: {
    //         if (board[y - 1][x] == 0) {
    //             possible_moves[y - 1][x] = true;
    //             if (board[y - 2][x] == 0 && y == 6) {
    //                 possible_moves[y - 2][x] = true;
    //             }
    //         } 
    //         if (x - 1 > 0 && board[y - 1][x - 1] < 0) possible_moves[y - 1][x - 1] = true;
    //         if (x + 1 < 8 && board[y - 1][x + 1] < 0) possible_moves[y - 1][x + 1] = true;
    //     } break;
    //     case w_KNIGHT: {
    //         if (board[y - 2][x + 1] <= 0) possible_moves[y - 2][x + 1] = true;
    //         if (board[y - 2][x - 1] <= 0) possible_moves[y - 2][x - 1] = true;
    //
    //         if (board[y - 1][x - 2] <= 0) possible_moves[y - 1][x - 2] = true;
    //         if (board[y - 1][x + 2] <= 0) possible_moves[y - 1][x + 2] = true;
    //
    //         if (board[y + 2][x + 1] <= 0) possible_moves[y + 2][x + 1] = true;
    //         if (board[y + 2][x - 1] <= 0) possible_moves[y + 2][x - 1] = true;
    //
    //         if (board[y + 1][x - 2] <= 0) possible_moves[y + 1][x - 2] = true;
    //         if (board[y + 1][x + 2] <= 0) possible_moves[y + 1][x + 2] = true;
    //     } break;
    //     case w_BISHOP: {
    //     } break;
    //     case w_ROOK: {
    //     } break;
    //     case w_QUEEN: {
    //     } break;
    //     case w_KING: {
    //     } break;
    //
    //     case b_PAWN: {
    //         if (y > 0 && y < 8) {
    //             if (board[y + 1][x] == 0) {
    //                 possible_moves[y + 1][x] = true;
    //                 if (board[y + 2][x] == 0 && y == 1) {
    //                     possible_moves[y + 2][x] = true;
    //                 }
    //             } 
    //             if (x - 1 > 0 && board[y + 1][x - 1] > 0) possible_moves[y + 1][x - 1] = true;
    //             if (x + 1 < 8 && board[y + 1][x + 1] > 0) possible_moves[y + 1][x + 1] = true;
    //         }
    //     } break;
    //     case b_KNIGHT: {
    //             if (board[y - 2][x + 1] >= 0) possible_moves[y - 2][x + 1] = true;
    //             if (board[y - 2][x - 1] >= 0) possible_moves[y - 2][x - 1] = true;
    //
    //             if (board[y - 1][x - 2] >= 0) possible_moves[y - 1][x - 2] = true;
    //             if (board[y - 1][x + 2] >= 0) possible_moves[y - 1][x + 2] = true;
    //
    //             if (board[y + 2][x + 1] >= 0) possible_moves[y + 2][x + 1] = true;
    //             if (board[y + 2][x - 1] >= 0) possible_moves[y + 2][x - 1] = true;
    //
    //             if (board[y + 1][x - 2] >= 0) possible_moves[y + 1][x - 2] = true;
    //             if (board[y + 1][x + 2] >= 0) possible_moves[y + 1][x + 2] = true;
    //     } break;
    //     case b_BISHOP: {
    //     } break;
    //     case b_ROOK: {
    //     } break;
    //     case b_QUEEN: {
    //     } break;
    //     case b_KING: {
    //     } break;
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
}
