#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#include "raylib.h"

#include "assets/b_pawn.h"
#include "assets/b_knight.h"
#include "assets/b_bishop.h"
#include "assets/b_rook.h"
#include "assets/b_queen.h"
#include "assets/b_king.h"

#include "assets/w_pawn.h"
#include "assets/w_knight.h"
#include "assets/w_bishop.h"
#include "assets/w_rook.h"
#include "assets/w_queen.h"
#include "assets/w_king.h"

#include "assets/brown.h"

#define WIDTH  512
#define HEIGHT 512
#define GRID_X 10
#define GRID_Y 10
#define GRID_SIZE GRID_X * GRID_Y
#define CELL_X 64
#define CELL_Y 64


typedef enum {
    e_WHITE,
    e_BLACK
} e_turn;


typedef enum {
    NONE,
    SELECT,
} e_state;


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

/*
   10 = white king
    9 = white queen
    5 = white rook
    4 = white bishop
    3 = white knight
    1 = white pawn

  -10 = black king
   -9 = black queen
   -5 = black rook
   -4 = black bishop
   -3 = black knight
   -1 = black pawn
*/
// e_piece board[GRID_X][GRID_Y] = {
//     {0, 0,       0,         0,        0,        0,      0,        0,        0,      0},
//     {0,  b_ROOK, b_KNIGHT,  b_BISHOP, b_QUEEN,  b_KING, b_BISHOP, b_KNIGHT, b_ROOK, 0},
//     {0,  b_PAWN, b_PAWN,    b_PAWN,   b_PAWN,   b_PAWN, b_PAWN,   b_PAWN,   w_PAWN, 0},
//     {0,  0,      0,         0,        0,        0,      0,        0,        0,      0},
//     {0,  0,      0,         0,        0,        0,      0,        0,        0,      0},
//     {0,  0,      0,         0,        0,        0,      0,        0,        0,      0},
//     {0,  0,      0,         0,        0,        0,      0,        0,        0,      0},
//     {0,  w_PAWN, w_PAWN,    w_PAWN,   w_PAWN,   w_PAWN, w_PAWN,   w_PAWN,   w_PAWN, 0},
//     {0,  b_PAWN, w_KNIGHT,  w_BISHOP, w_QUEEN,  w_KING, w_BISHOP, w_KNIGHT, w_ROOK, 0},
//     {0,  0,      0,         0,        0,        0,      0,        0,        0,      0},
// };

e_piece board[GRID_X][GRID_Y] = {
    {0, 0,      0,        0,        0,        0,      0,         0,       0,      0},
    {0, b_ROOK, b_KNIGHT, b_BISHOP, b_QUEEN, b_KING, b_BISHOP,  b_KNIGHT, b_ROOK, 0},
    {0, b_PAWN, b_PAWN,   b_PAWN,   b_PAWN,  b_PAWN, b_PAWN,    b_PAWN,   w_PAWN, 0},
    {0,  0,     0,        0,        0,       0,      0,         0,        0     , 0},
    {0,  0,     0,        0,        0,       0,      0,         0,        0     , 0},
    {0,  0,     0,        0,        0,       0,      0,         0,        0     , 0},
    {0, 0,     0,        b_PAWN,        0,       0,      0,         0,        0 , 0},
    {0, w_PAWN, w_PAWN,   w_PAWN,   w_PAWN,  w_PAWN, w_PAWN,    w_PAWN,   w_PAWN, 0},
    {0, b_PAWN, w_KNIGHT, w_BISHOP, w_QUEEN, w_KING, w_BISHOP,  w_KNIGHT, w_ROOK, 0},
    {0, 0,      0,        0,        0,        0,      0,         0,       0,      0},
};

bool selected_possible_moves[GRID_X][GRID_Y] = {0};
void reset_possible_moves(void)
{
    for (size_t i = 0; i < GRID_Y; i++) {
        for (size_t j = 0; j < GRID_X; j++) {
            selected_possible_moves[i][j] = false;
        }
    }
}
void find_possible_moves(size_t x, size_t y, e_piece type)
{
    switch (type) {
        case e_EMPTY: {
            reset_possible_moves();
        } break;
        case w_PAWN: {
            if (y > 0 && y < 8) {
                if (board[y - 1][x] == 0) {
                    selected_possible_moves[y - 1][x] = true;
                    if (board[y - 2][x] == 0 && y == 7) {
                        selected_possible_moves[y - 2][x] = true;
                    }
                } 
                if (x - 1 > 0 && board[y - 1][x - 1] < 0) selected_possible_moves[y - 1][x - 1] = true;
                if (x + 1 < 8 && board[y - 1][x + 1] < 0) selected_possible_moves[y - 1][x + 1] = true;
            }
        } break;
        case w_KNIGHT: {
            if (board[y - 2][x + 1] <= 0) selected_possible_moves[y - 2][x + 1] = true;
            if (board[y - 2][x - 1] <= 0) selected_possible_moves[y - 2][x - 1] = true;

            if (board[y - 1][x - 2] <= 0) selected_possible_moves[y - 1][x - 2] = true;
            if (board[y - 1][x + 2] <= 0) selected_possible_moves[y - 1][x + 2] = true;

            if (board[y + 2][x + 1] <= 0) selected_possible_moves[y + 2][x + 1] = true;
            if (board[y + 2][x - 1] <= 0) selected_possible_moves[y + 2][x - 1] = true;

            if (board[y + 1][x - 2] <= 0) selected_possible_moves[y + 1][x - 2] = true;
            if (board[y + 1][x + 2] <= 0) selected_possible_moves[y + 1][x + 2] = true;
        } break;
        case w_BISHOP: {
        } break;
        case w_ROOK: {
        } break;
        case w_QUEEN: {
        } break;
        case w_KING: {
        } break;

        case b_PAWN: {
            if (y > 0 && y < 8) {
                if (board[y + 1][x] == 0) {
                    selected_possible_moves[y + 1][x] = true;
                    if (board[y + 2][x] == 0 && y == 2) {
                        selected_possible_moves[y + 2][x] = true;
                    }
                } 
                if (x - 1 > 0 && board[y + 1][x - 1] > 0) selected_possible_moves[y + 1][x - 1] = true;
                if (x + 1 < 8 && board[y + 1][x + 1] > 0) selected_possible_moves[y + 1][x + 1] = true;
            }
        } break;
        case b_KNIGHT: {
                if (board[y - 2][x + 1] >= 0) selected_possible_moves[y - 2][x + 1] = true;
                if (board[y - 2][x - 1] >= 0) selected_possible_moves[y - 2][x - 1] = true;

                if (board[y - 1][x - 2] >= 0) selected_possible_moves[y - 1][x - 2] = true;
                if (board[y - 1][x + 2] >= 0) selected_possible_moves[y - 1][x + 2] = true;

                if (board[y + 2][x + 1] >= 0) selected_possible_moves[y + 2][x + 1] = true;
                if (board[y + 2][x - 1] >= 0) selected_possible_moves[y + 2][x - 1] = true;

                if (board[y + 1][x - 2] >= 0) selected_possible_moves[y + 1][x - 2] = true;
                if (board[y + 1][x + 2] >= 0) selected_possible_moves[y + 1][x + 2] = true;
        } break;
        case b_BISHOP: {
        } break;
        case b_ROOK: {
        } break;
        case b_QUEEN: {
        } break;
        case b_KING: {
        } break;
    
    }
}


int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Chess");
    Image board_background_img = LoadImageFromMemory(".png", brown_resized_png, brown_resized_png_len);
    Texture board_background = LoadTextureFromImage(board_background_img);
    
    Image w_pawn_img = LoadImageFromMemory(".png", w_pawn_png, w_pawn_png_len);
    Texture w_pawn = LoadTextureFromImage(w_pawn_img);

    Image w_knight_img = LoadImageFromMemory(".png", w_knight_png, w_knight_png_len);
    Texture w_knight = LoadTextureFromImage(w_knight_img);

    Image w_rook_img = LoadImageFromMemory(".png", w_rook_png, w_rook_png_len);
    Texture w_rook = LoadTextureFromImage(w_rook_img);

    Image w_bishop_img = LoadImageFromMemory(".png", w_bishop_png, w_bishop_png_len);
    Texture w_bishop = LoadTextureFromImage(w_bishop_img);

    Image w_queen_img = LoadImageFromMemory(".png", w_queen_png, w_queen_png_len);
    Texture w_queen = LoadTextureFromImage(w_queen_img);
    
    Image w_king_img = LoadImageFromMemory(".png", w_king_png, w_king_png_len);
    Texture w_king = LoadTextureFromImage(w_king_img);


    Image b_pawn_img = LoadImageFromMemory(".png", b_pawn_png, b_pawn_png_len);
    Texture b_pawn = LoadTextureFromImage(b_pawn_img);

    Image b_knight_img = LoadImageFromMemory(".png", b_knight_png, b_knight_png_len);
    Texture b_knight = LoadTextureFromImage(b_knight_img);

    Image b_rook_img = LoadImageFromMemory(".png", b_rook_png, b_rook_png_len);
    Texture b_rook = LoadTextureFromImage(b_rook_img);

    Image b_bishop_img = LoadImageFromMemory(".png", b_bishop_png, b_bishop_png_len);
    Texture b_bishop = LoadTextureFromImage(b_bishop_img);

    Image b_queen_img = LoadImageFromMemory(".png", b_queen_png, b_queen_png_len);
    Texture b_queen = LoadTextureFromImage(b_queen_img);
    
    Image b_king_img = LoadImageFromMemory(".png", b_king_png, b_king_png_len);
    Texture b_king = LoadTextureFromImage(b_king_img);

    e_turn turn = e_WHITE;
    e_state state = NONE;

    size_t selected_x = 0;
    size_t selected_y = 0;
    e_piece selected_type = e_EMPTY;

    while (!WindowShouldClose()) { 
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse_pos = GetMousePosition();
            size_t x = ((size_t)mouse_pos.x / CELL_X) + 1;
            size_t y = ((size_t)mouse_pos.y / CELL_Y) + 1;
            if (state == SELECT && x == selected_x && y == selected_y) {
                state = NONE;
                selected_type = e_EMPTY;
                reset_possible_moves();
            }
            else if (state == SELECT) {
                if (selected_possible_moves[y][x]) {
                    board[y][x] = board[selected_y][selected_x];
                    board[selected_y][selected_x] = 0;
                }
                selected_x = 0;
                selected_y = 0;
                state = NONE;
                selected_type = e_EMPTY;
                reset_possible_moves();
            }
            else if (board[y][x] != 0 && state == NONE) {
                state = SELECT;
                selected_x = x;
                selected_y = y;
                selected_type = board[y][x];
            }
        }
        BeginDrawing();
            ClearBackground(WHITE);
            DrawTexture(board_background, 0, 0, WHITE);
            for (size_t i = 1; i < GRID_Y - 1; i++) {
                for (size_t j = 1; j < GRID_X - 1; j++) {
                    switch (board[i][j]) {
                        case e_EMPTY: {
                            continue;
                        } break;
                        case w_PAWN: {
                            DrawTextureEx(w_pawn, (Vector2) {((j - 1) * CELL_X) + 11, ((i - 1) * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case w_KNIGHT: {
                            DrawTextureEx(w_knight, (Vector2) {((j - 1) * CELL_X) + 8, ((i - 1) * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case w_BISHOP: {
                            DrawTextureEx(w_bishop, (Vector2) {((j - 1) * CELL_X) + 7, ((i - 1) * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case w_ROOK: {
                            DrawTextureEx(w_rook, (Vector2) {((j - 1) * CELL_X) + 10, ((i - 1) * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case w_QUEEN: {
                            DrawTextureEx(w_queen, (Vector2) {((j - 1) * CELL_X) + 5, ((i - 1) * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case w_KING: {
                            DrawTextureEx(w_king, (Vector2) {((j - 1) * CELL_X) + 7, ((i - 1) * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;

                        case b_PAWN: {
                            DrawTextureEx(b_pawn, (Vector2) {((j - 1) * CELL_X) + 11, ((i - 1) * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case b_KNIGHT: {
                            DrawTextureEx(b_knight, (Vector2) {((j - 1) * CELL_X) + 8, ((i - 1) * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case b_BISHOP: {
                            DrawTextureEx(b_bishop, (Vector2) {((j - 1) * CELL_X) + 7, ((i - 1) * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case b_ROOK: {
                            DrawTextureEx(b_rook, (Vector2) {((j - 1) * CELL_X) + 10, ((i - 1) * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case b_QUEEN: {
                            DrawTextureEx(b_queen, (Vector2) {((j - 1) * CELL_X) + 5, ((i - 1) * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case b_KING: {
                            DrawTextureEx(b_king, (Vector2) {((j - 1) * CELL_X) + 7, ((i - 1) * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;

                    }
                } 
            }

            switch (state) {
                case NONE: {

                } break;

                case SELECT: {
                    find_possible_moves(selected_x, selected_y, selected_type);
                    for (size_t i = 1; i < GRID_Y - 1; i++) {
                        for (size_t j = 1; j < GRID_X - 1; j++) {
                            if (selected_possible_moves[i][j]) {
                                DrawCircle(((j - 1) * CELL_X) + CELL_X/2, ((i - 1) * CELL_Y) + CELL_Y/2, 5, GRAY);
                            }
                        }
                    }
                }; break;

                default: {

                } break;
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
