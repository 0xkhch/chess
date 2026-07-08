#include "draw.h"

//TODO: make these dynamic ?
#define LIGHT_SHADE (Color){.r = 0xFF, .g = 0xCF, .b = 0x9F, .a = 0xFF}
#define DARK_SHADE (Color){.r = 0xD2, .g = 0x8C, .b = 0x45, .a = 0xFF}

#define LIGHT_GREEN (Color){.r = 0xAE, .g = 0xB1, .b = 0x87, .a = 0xFF}
#define DARK_GREEN (Color){.r = 0x84, .g = 0x79, .b = 0x4e, .a = 0xFF}

#define DARK_GREEN (Color){.r = 0x84, .g = 0x79, .b = 0x4e, .a = 0xFF}
#define TRANS_RED_DARK  (Color){.r = 0xBE, .g = 0x21, .b = 0x37, .a = 0xB0}
#define TRANS_PURPLE_DARK  (Color){.r = 0x37, .g = 0x21, .b = 0xBE, .a = 0xB0}

bool is_checked(e_piece type);

Rectangle type_to_rect(e_piece type)
{
#define PIECE_WIDTH ((float)768/6)
#define PIECE_HEIGHT ((float)256/2)

    switch (type) {
        case e_EMPTY: {
            return (Rectangle){0.0f, 0.0f,   PIECE_WIDTH, PIECE_HEIGHT};
        } break;
        case b_PAWN: {
            return (Rectangle){0.0f, 0.0f,   PIECE_WIDTH, PIECE_HEIGHT};
        } break;
        case b_KNIGHT: {
            return (Rectangle){128.0f, 0.0f, PIECE_WIDTH, PIECE_HEIGHT};
        } break;
        case b_BISHOP: {
            return (Rectangle){256.0f, 0.0f, PIECE_WIDTH, PIECE_HEIGHT};
        } break;
        case b_ROOK: {
            return (Rectangle){384.0f, 0.0f, PIECE_WIDTH, PIECE_HEIGHT};
        } break;
        case b_QUEEN: {
            return (Rectangle){512.0f, 0.0f, PIECE_WIDTH, PIECE_HEIGHT};
        } break;
        case b_KING: {
            return (Rectangle){640.0f, 0.0f, PIECE_WIDTH, PIECE_HEIGHT};
        } break;

        case w_PAWN: {
            return (Rectangle){0.0f,   128.0f, PIECE_WIDTH, PIECE_HEIGHT};
        } break;
        case w_KNIGHT: {
            return (Rectangle){128.0f, 128.0f, PIECE_WIDTH, PIECE_HEIGHT};
        } break;
        case w_BISHOP: {
            return (Rectangle){256.0f, 128.0f, PIECE_WIDTH, PIECE_HEIGHT};
        } break;
        case w_ROOK: {
            return (Rectangle){384.0f, 128.0f, PIECE_WIDTH, PIECE_HEIGHT};
        } break;
        case w_QUEEN: {
            return (Rectangle){512.0f, 128.0f, PIECE_WIDTH, PIECE_HEIGHT};
        } break;
        case w_KING: {
            return (Rectangle){640.0f, 128.0f, PIECE_WIDTH, PIECE_HEIGHT};
        } break;
    }
    return (Rectangle){0.0f, 0.0f,   PIECE_WIDTH, PIECE_HEIGHT}; // suppress stupid warning
}

void draw_board(Texture2D* pieces)
{
    Rectangle texture_dest = {.x = 0.0f, .y = 0.0f, .width = 64.0f, .height = 64.0f};
    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            // draw board
            bool is_light = ((j + i) % 2 == 0);
            bool possible_move = (bool)get_bit(global.selected_moves, j, i);
            if(global.state == s_SELECT && possible_move && global.mouse_pos_x == j && global.mouse_pos_y == i) {
                DrawRectangle(j * CELL_X, i * CELL_Y, CELL_X, CELL_Y, is_light ? LIGHT_GREEN: DARK_GREEN);
            }
            else {
                DrawRectangle(j * CELL_X, i * CELL_Y, CELL_X, CELL_Y, is_light ? LIGHT_SHADE : DARK_SHADE);
            }
#ifdef DEBUG
            bool heat = (bool)get_bit(global.heatmap, j, i);
            if (heat) {
                DrawRectangle(j * CELL_X, i * CELL_Y, CELL_X, CELL_Y, TRANS_PURPLE_DARK);
            }
#endif /* ifdef DEBUG */

            e_piece type = board[i * GRID_X + j];
            if (is_checked(type)) {
                DrawRectangle(j * CELL_X, i * CELL_Y, CELL_X, CELL_Y, TRANS_RED_DARK);
            }

            // draw pieces
            if (type != e_EMPTY) {
                Vector2 pos = (Vector2) {
                    .x = (j * CELL_X) - (128 * j), 
                    .y = (i * CELL_Y) - (128 * i)
                };
#ifdef DEBUG
                DrawText(TextFormat("x: %d y: %d", j, i), j * CELL_X, i * CELL_Y, 16, RED);
#endif /* ifdef DEBUG */
                DrawTexturePro(*pieces, type_to_rect(type), texture_dest, pos, 0, WHITE);
            }

            // draw circles
            if (possible_move) {
#ifdef DEBUG
                DrawText(TextFormat("x: %d y: %d", j, i), j * CELL_X, i * CELL_Y, 16, RED);
#endif /* ifdef DEBUG */
                DrawCircle((j * CELL_X) + CELL_X/2, (i * CELL_Y) + CELL_Y/2, 5, DARK_GREEN);
            }
        }
    }
}

bool is_checked(e_piece type)
{
    return ((type == b_KING && global.checked == c_BLACK) || (type == w_KING && global.checked == c_WHITE)) && global.amount_checked > 0;
}
