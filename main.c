#include <stdio.h>
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
#define GRID_X 8
#define GRID_Y 8
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
e_piece board[GRID_X][GRID_Y] = {
    {b_ROOK, b_KNIGHT, b_BISHOP, b_QUEEN, b_KING, b_BISHOP,  b_KNIGHT, b_ROOK},
    {b_PAWN, b_PAWN,   b_PAWN,   b_PAWN,  b_PAWN, b_PAWN,    b_PAWN,   b_PAWN},
    { 0,     0,        0,        0,       0,      0,         0,        0     },
    { 0,     0,        0,        0,       0,      0,         0,        0     },
    { 0,     0,        0,        0,       0,      0,         0,        0     },
    { 0,     0,        0,        0,       0,      0,         0,        0     },
    {w_PAWN, w_PAWN,   w_PAWN,   w_PAWN,  w_PAWN, w_PAWN,    w_PAWN,   w_PAWN},
    {w_ROOK, w_KNIGHT, w_BISHOP, w_QUEEN, w_KING, w_BISHOP,  w_KNIGHT, w_ROOK },
};


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
    Vector2 current_selected = {0, 0};
    while (!WindowShouldClose()) { 
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse_pos = GetMousePosition();
            size_t x = (size_t)mouse_pos.x / CELL_X;
            size_t y = (size_t)mouse_pos.y / CELL_Y;
            if (state == SELECT && x == current_selected.x && y == current_selected.y) {
                state = NONE;
            }
            else if (state == SELECT) {
                board[y][x] = board[(int)current_selected.y][(int)current_selected.x];
                board[(int)current_selected.y][(int)current_selected.x] = 0;
                current_selected.x = 0;
                current_selected.y = 0;
                state = NONE;
            }
            else if (board[y][x] != 0 && state == NONE) {
                state = SELECT;
                current_selected.x = x;
                current_selected.y = y;
            }
        }
        BeginDrawing();
            ClearBackground(WHITE);
            DrawTexture(board_background, 0, 0, WHITE);
            for (size_t i = 0; i < GRID_X; i++) {
                for (size_t j = 0; j < GRID_Y; j++) {
                    switch (board[i][j]) {
                        case e_EMPTY: {
                            continue;
                        } break;
                        case w_PAWN: {
                            DrawTextureEx(w_pawn, (Vector2) {(j * CELL_X) + 10, (i * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case w_KNIGHT: {
                            DrawTextureEx(w_knight, (Vector2) {(j * CELL_X) + 8, (i * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case w_BISHOP: {
                            DrawTextureEx(w_bishop, (Vector2) {(j * CELL_X) + 7, (i * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case w_ROOK: {
                            DrawTextureEx(w_rook, (Vector2) {(j * CELL_X) + 10, (i * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case w_QUEEN: {
                            DrawTextureEx(w_queen, (Vector2) {(j * CELL_X) + 5, (i * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case w_KING: {
                            DrawTextureEx(w_king, (Vector2) {(j * CELL_X) + 7, (i * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;

                        case b_PAWN: {
                            DrawTextureEx(b_pawn, (Vector2) {(j * CELL_X) + 10, (i * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case b_KNIGHT: {
                            DrawTextureEx(b_knight, (Vector2) {(j * CELL_X) + 8, (i * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case b_BISHOP: {
                            DrawTextureEx(b_bishop, (Vector2) {(j * CELL_X) + 7, (i * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case b_ROOK: {
                            DrawTextureEx(b_rook, (Vector2) {(j * CELL_X) + 10, (i * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case b_QUEEN: {
                            DrawTextureEx(b_queen, (Vector2) {(j * CELL_X) + 5, (i * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;
                        case b_KING: {
                            DrawTextureEx(b_king, (Vector2) {(j * CELL_X) + 7, (i * CELL_Y) + 5}, 0, 0.4 , WHITE);
                        } break;

                    }
                } 
            }

            // switch (state) {
            //     case NONE: {
            //
            //     } break;
            //
            //     case SELECT: {
            //         /*
            //          * For all valid positions of selected piece
            //          * DrawCircle((current_selected.x * CELL_X) + CELL_X/2, ((current_selected.y - 1) * CELL_Y) + CELL_Y/2, 5, GRAY);
            //          * 
            //          */
            //     }; break;
            //
            //     default: {
            //
            //     } break;
            // }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
