#include "common.h"
#include "raylib.h"
#include "board.h"

// textures
#include "pieces.h"
#include "move.h"
#include "capture.h"

typedef struct global_state {
    int selected_x;
    int selected_y;
    int mouse_pos_x;
    int mouse_pos_y;
    int mouse_pos;
    e_piece selected_type;
    e_state state;
    bool turn;
} global_state_t;
global_state_t global = {};

//TODO: make these dynamic ?
#define LIGHT_SHADE (Color){.r = 0xFF, .g = 0xCF, .b = 0x9F, .a = 0xFF}
#define DARK_SHADE (Color){.r = 0xD2, .g = 0x8C, .b = 0x45, .a = 0xFF}

#define LIGHT_GREEN (Color){.r = 0xAE, .g = 0xB1, .b = 0x87, .a = 0xFF}
#define DARK_GREEN (Color){.r = 0x84, .g = 0x79, .b = 0x4e, .a = 0xFF}

#define t_WHITE false
#define t_BLACK true


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

            bool possible_move = possible_moves[i * GRID_Y + j];
            if(global.state == SELECT && possible_move && global.mouse_pos_x == j && global.mouse_pos_y == i) {
                DrawRectangle(j * CELL_X, i * CELL_Y, CELL_X, CELL_Y, is_light ? LIGHT_GREEN: DARK_GREEN);
            }
            else {
                DrawRectangle(j * CELL_X, i * CELL_Y, CELL_X, CELL_Y, is_light ? LIGHT_SHADE : DARK_SHADE);
            }
            
            // draw pieces
            e_piece type = board[i * GRID_X + j];
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
                DrawCircle((j * CELL_X) + CELL_X/2, (i * CELL_Y) + CELL_Y/2, 5, RED);
                DrawText(TextFormat("x: %d y: %d", j, i), j * CELL_X, i * CELL_Y, 16, RED);
#else
                DrawCircle((j * CELL_X) + CELL_X/2, (i * CELL_Y) + CELL_Y/2, 5, DARK_GREEN);
#endif /* ifdef DEBUG */
            }
        }
    }
}

int main(int argc, char** argv)
{
    if (argc > 3) {
        fprintf(stderr, "%s\n", "Too many arguments...");
        return 1;
    }
    else if (argc == 3) {
        init_board(argv[argc - 2]);
        switch (*argv[argc - 1]) {
            case 'b':
                global.turn = true;
            break;
            case 'w':
                global.turn = false;
            break;
        }
    }
    else {
#ifdef DEBUG
        init_board("K5Nk/8/8/8/3kQ3/8/8/k6K");
#else
        init_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
#endif /* ifdef DEBUG */
    }

    InitWindow(WIDTH, HEIGHT, "Chess");
    InitAudioDevice();

    Image pieces_img = LoadImageFromMemory(".png", pieces_png, pieces_png_len);
    Wave move_wav    = LoadWaveFromMemory(".mp3", move_mp3, move_mp3_len);
    Wave capture_wav    = LoadWaveFromMemory(".mp3", capture_mp3, capture_mp3_len);
    Texture2D pieces   = LoadTextureFromImage(pieces_img);
    Sound move_sound = LoadSoundFromWave(move_wav);
    Sound capture_sound = LoadSoundFromWave(capture_wav);

    bool hovering_possible= false;
    int x = 0;
    int y = 0;

    SetTargetFPS(165);
    while (!WindowShouldClose()) { 
        Vector2 mouse_pos = GetMousePosition();
        x = (mouse_pos.x / CELL_X);
        y = (mouse_pos.y / CELL_Y);
        global.mouse_pos_x = x;
        global.mouse_pos_y = y;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            switch (global.state) {
                case NONE: {
#ifdef DEBUG
                    reset_possible_moves();
                    if (board[y * GRID_X + x] == 0) {
                        global.state = NONE;
                        global.selected_type = e_EMPTY;
                        global.selected_x = 0;
                        global.selected_y = 0;
                    }
                    else {
                        global.state = SELECT;
                        global.selected_x = x;
                        global.selected_y = y;
                        global.selected_type = board[y * GRID_X + x];
                    }
#else
                    if ((board[y * GRID_X + x] == 0 
                        || (board[y * GRID_X + x] < 0 && global.turn == t_WHITE) 
                        || (board[y * GRID_X + x] > 0 && global.turn == t_BLACK))) {
                        global.state = NONE;
                        global.selected_type = e_EMPTY;
                        global.selected_x = 0;
                        global.selected_y = 0;
                        reset_possible_moves();
                    }
                    else if ((board[y * GRID_X + x] > 0 && global.turn == t_WHITE) 
                            || (board[y * GRID_X + x] < 0 && global.turn == t_BLACK)) {
                        global.state = SELECT;
                        global.selected_x = x;
                        global.selected_y = y;
                        global.selected_type = board[y * GRID_X + x];
                    }
#endif /* ifdef DEBUG */
                } break;
                case SELECT: {
#ifdef DEBUG
                    if (possible_moves[y * GRID_X + x]) {
                        global.state = NONE;
                        board[y * GRID_X + x] != e_EMPTY ? PlaySound(capture_sound) : PlaySound(move_sound);
                        board[y * GRID_X + x] = board[global.selected_y * GRID_X + global.selected_x];
                        board[global.selected_y * GRID_X + global.selected_x] = e_EMPTY;
                        global.turn = !global.turn;

                        global.selected_type = e_EMPTY;
                        global.selected_x = 0;
                        global.selected_y = 0;
                    }
                    else {
                        global.state = NONE;
                    }
                    reset_possible_moves();
#else
                    if ((global.selected_y == y && global.selected_x == x) && ((board[y * GRID_X + x] > 0 && global.turn == t_WHITE) 
                        || (board[y * GRID_X + x] < 0 && global.turn == t_BLACK))) {
                        global.state = NONE;
                        global.selected_type = e_EMPTY;
                        global.selected_x = 0;
                        global.selected_y = 0;
                        break;
                    }
                    if ((board[y * GRID_X + x] > 0 && global.turn == t_WHITE) 
                        || (board[y * GRID_X + x] < 0 && global.turn == t_BLACK)) {
                        global.state = SELECT;
                        global.selected_x = x;
                        global.selected_y = y;
                        global.selected_type = board[y * GRID_X + x];
                    }
                    else if (!possible_moves[y * GRID_X + x] && ((board[y * GRID_X + x] < 0 && global.turn == t_WHITE) 
                            || (board[y * GRID_X + x] > 0 && global.turn == t_BLACK))) {
                        global.state = NONE;
                        global.selected_type = e_EMPTY;
                        global.selected_x = 0;
                        global.selected_y = 0;
                    }
                    else if ((possible_moves[y * GRID_X + x] 
                            && ((board[y * GRID_X + x] <= 0 && global.turn == t_WHITE) 
                            || (board[y * GRID_X + x] >= 0 && global.turn == t_BLACK)))) {
                        global.state = NONE;
                        board[y * GRID_X + x] != e_EMPTY ? PlaySound(capture_sound) : PlaySound(move_sound);
                        board[y * GRID_X + x] = board[global.selected_y * GRID_X + global.selected_x];
                        board[global.selected_y * GRID_X + global.selected_x] = e_EMPTY;
                        global.turn = !global.turn;
                        global.selected_type = e_EMPTY;
                        global.selected_x = 0;
                        global.selected_y = 0;
                    }
                    reset_possible_moves();
#endif /* ifdef DEBUG */
                } break;
                case HOLD: {
                } break;
            }
        }
        BeginDrawing();
            draw_board(&pieces);
            ClearBackground(WHITE);
            switch (global.state) {
                case NONE: {
                } break;
                case SELECT: {
                    // TODO: compute this once mayhaps
                    int num = find_possible_moves(global.selected_x, global.selected_y, global.selected_type);
                }; break;
                default: {
                } break;
            }
            DrawFPS(0, 0);
        EndDrawing();
    }
    UnloadTexture(pieces);
    CloseWindow();
    return 0;
}
