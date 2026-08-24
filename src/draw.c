#include "draw.h"

//TODO: make these dynamic ?
#define LIGHT_SHADE (Color){.r = 0xFF, .g = 0xCF, .b = 0x9F, .a = 0xFF}
#define DARK_SHADE (Color){.r = 0xD2, .g = 0x8C, .b = 0x45, .a = 0xFF}

#define LIGHT_GREEN (Color){.r = 0xAE, .g = 0xB1, .b = 0x87, .a = 0xFF} // #AEB187
#define INBETWEEN_GREEN (Color){.r = 0x9B, .g = 0x98, .b = 0x6D, .a = 0xFF} // #9B986D
// #define INBETWEEN_GREEN (Color){.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF} // #9B986D
#define DARK_GREEN (Color){.r = 0x84, .g = 0x79, .b = 0x4e, .a = 0xFF} // #84794E


#define TRANS_RED_DARK  (Color){.r = 0xBE, .g = 0x21, .b = 0x37, .a = 0xB0}
#define TRANS_PURPLE_DARK  (Color){.r = 0x37, .g = 0x21, .b = 0xBE, .a = 0xB0}

bool is_checked(e_piece type);
bool is_opposite(e_piece piece, bool turn);

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
            if (global.state == s_SELECT && 
                possible_move && 
                (is_opposite(board[i * GRID_Y + j], global.turn) || board[i * GRID_Y + j] == e_EMPTY) && 
                global.mouse_pos_x == j && 
                global.mouse_pos_y == i
            ) {
                DrawRectangle(j * CELL_X, i * CELL_Y, CELL_X, CELL_Y, is_light ? LIGHT_GREEN: DARK_GREEN);
            }
            else {
                DrawRectangle(j * CELL_X, i * CELL_Y, CELL_X, CELL_Y, is_light ? LIGHT_SHADE : DARK_SHADE);
            }

            if (global.prev.type != e_EMPTY && global.screen_state == PLAY) {
                if ((global.prev.fx == j && global.prev.fy == i) || (global.prev.tx == j && global.prev.ty == i)) {
                    DrawRectangle(j * CELL_X, i * CELL_Y, CELL_X, CELL_Y, is_light ? LIGHT_GREEN: DARK_GREEN);
                }
            }
#ifdef DEBUG
            bool heat = (bool)get_bit(global.heatmap, j, i);
            if (heat && global.screen_state == PLAY) {
                DrawRectangle(j * CELL_X, i * CELL_Y, CELL_X, CELL_Y, TRANS_PURPLE_DARK);
            }
#endif /* ifdef DEBUG */

            e_piece type = board[i * GRID_X + j];
            if (is_checked(type) && global.screen_state == PLAY) {
                DrawRectangle(j * CELL_X, i * CELL_Y, CELL_X, CELL_Y, TRANS_RED_DARK);
            }

            // draw pieces
            if (type != e_EMPTY && global.screen_state == PLAY) {
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
            if (possible_move &&
                (is_opposite(board[i * GRID_Y + j], global.turn) || board[i * GRID_Y + j] == e_EMPTY) && 
                global.screen_state == PLAY
            ) {
#ifdef DEBUG
                DrawText(TextFormat("x: %d y: %d", j, i), j * CELL_X, i * CELL_Y, 16, RED);
#endif /* ifdef DEBUG */
                DrawCircle((j * CELL_X) + CELL_X/2, (i * CELL_Y) + CELL_Y/2, 5, INBETWEEN_GREEN);
            }
        }
    }
}

void draw_intro()
{
    static unsigned int frame = 0;
    ClearBackground(LIGHT_SHADE);
    if (frame == 0xFF) global.screen_state = MENU;
    char* text = "khanafis";
    int font_size = 64;
    int text_size = MeasureText(text, font_size);
    Color color = (Color){.r = 0x00, .g = 0x00, .b = 0x00, .a = frame};
    DrawText(text, WIDTH/2 - text_size/2, HEIGHT/2 - font_size, font_size, color);
    frame++;
}

void draw_menu(Texture2D* pieces)
{
    draw_board(pieces);
    char* title = "Chess";
    int t_font_size = 64;
    DrawText(title, 0, HEIGHT/2 - (t_font_size * 2 - 13), t_font_size, BLACK);

    char* option = "Play game";
    int o_font_size = 39;
    if (global.mouse_pos_x >= 2 && global.mouse_pos_x <= 5 && global.mouse_pos_y == 4) {
        o_font_size = 44;
        global.inside_option = true;
    }
    else {
        global.inside_option = false;
    }
    int o_text_size = MeasureText(option, o_font_size);
    DrawText(option, WIDTH/2 - o_text_size/2, HEIGHT/2 - (o_font_size/2) + CELL_Y/2, o_font_size, WHITE);

    Vector2 pos = (Vector2) {
        .x = 32.0f,
        .y = 32.0f
    };

    Rectangle texture_dest_w = {.x = 238.0f, .y = 160.0f, .width = 64.0f, .height = 64.0f};
    DrawTexturePro(*pieces, type_to_rect(w_PAWN), texture_dest_w, pos, 30, WHITE);

    Rectangle texture_dest_b = {.x = 206.0f, .y = 160.0f, .width = 64.0f, .height = 64.0f};
    DrawTexturePro(*pieces, type_to_rect(b_PAWN), texture_dest_b, pos, -30, WHITE);
}

void draw_promo(Texture2D* pieces)
{
    draw_board(pieces);

    const int y = 4;
    const int start_x = 2;

    int white_pieces[4] = {w_QUEEN, w_ROOK, w_BISHOP, w_KNIGHT};
    int black_pieces[4] = {b_QUEEN, b_ROOK, b_BISHOP, b_KNIGHT};

    int* piece_set = global.turn == t_WHITE ? white_pieces : black_pieces;

    for (int i = 0; i < 4; i++)
    {
        int board_x = start_x + i;
        bool hovered = (global.mouse_pos_x == board_x &&
                        global.mouse_pos_y == y);

        Rectangle dest = {
            .x = hovered ? -3.0f : 0.0f,
            .y = hovered ? -6.0f : 0.0f,
            .width  = hovered ? 70.0f : 64.0f,
            .height = hovered ? 70.0f : 64.0f
        };

        Vector2 pos = {
            .x = (board_x * CELL_X) - (128 * board_x),
            .y = (y * CELL_Y) - (128 * y)
        };

        DrawTexturePro(*pieces,
                       type_to_rect(piece_set[i]),
                       dest,
                       pos,
                       0.0f,
                       WHITE);
    }
}
void draw_end(Texture2D* pieces)
{
    draw_board(pieces);
    bool winner = !global.turn;
    if (global.draw) {
        char* title = "Draw!";
        int t_font_size = 64;
        int t_text_size = MeasureText(title, t_font_size);
        DrawText(title, WIDTH/2 - t_text_size/2, HEIGHT/2 - (t_font_size * 3 - 13), t_font_size, BLACK);
    }
    else {
        int t_font_size = 64;
        if (winner == t_WHITE) {
            char* title = "White won!";
            int t_text_size = MeasureText(title, t_font_size);
            t_text_size = MeasureText(title, t_font_size);
            DrawText(title, WIDTH/2 - t_text_size/2, HEIGHT/2 - (t_font_size * 3 - 13), t_font_size, BLACK);
        }
        else {
            char* title = "Black won!";
            int t_text_size = MeasureText(title, t_font_size);
            t_text_size = MeasureText(title, t_font_size);
            DrawText(title, WIDTH/2 - t_text_size/2, HEIGHT/2 - (t_font_size * 3 - 13), t_font_size, BLACK);

        }
    }

    // y = 4 x = 3
    Vector2 pos = (Vector2) {
        .x = 32.0f,
        .y = 32.0f
    };
    float texture_x = 64.0f * 4 + 32.0f;
    Rectangle texture_dest_w = {.x = texture_x - 64.0f, .y = 64.0f * 3 + 32.0f, .width = 64.0f, .height = 64.0f};
    DrawTexturePro(*pieces, type_to_rect(b_KING), texture_dest_w, pos, 0, WHITE);

    Rectangle texture_dest_b = {.x = texture_x, .y = 64.0f * 4 + 32.0f, .width = 64.0f, .height = 64.0f};
    DrawTexturePro(*pieces, type_to_rect(w_KING), texture_dest_b, pos, 0, WHITE);
}

void draw_error(Texture2D* pieces)
{
    draw_board(pieces);
    char* title = "whoopsie...";
    int t_font_size = 64;
    int t_text_size = MeasureText(title, t_font_size);
    DrawText(title, WIDTH/2 - t_text_size/2, HEIGHT/2  - (t_font_size/2), t_font_size, BLACK);
}