#ifndef PIECE_H_
#define PIECE_H_

typedef enum {
    s_NONE,
    s_SELECT,
} e_state;

typedef enum {
    c_WHITE,
    c_BLACK
} e_check;


typedef enum {
    e_EMPTY = 0,
    w_PAWN   = 1,
    w_KNIGHT = 3,
    w_BISHOP = 4,
    w_ROOK   = 5,
    w_QUEEN  = 9,
    w_KING   = 10,

    b_PAWN   = -1,
    b_KNIGHT = -3,
    b_BISHOP = -4,
    b_ROOK   = -5,
    b_QUEEN  = -9,
    b_KING   = -10,
} e_piece;

#endif // !PIECE_H_
