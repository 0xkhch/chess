#include "common.h"
#include "raylib.h"
#include "board.h"

#include "pieces.h"
#include "move.h"
#include "capture.h"

//TODO: make these dynamic ?
#define LIGHT_SHADE (Color){.r = 0xFF, .g = 0xCF, .b = 0x9F, .a = 0xFF}
#define DARK_SHADE (Color){.r = 0xD2, .g = 0x8C, .b = 0x45, .a = 0xFF}

void draw_board(void)
{
    for (size_t i = 0; i < WIDTH; i++) {
        size_t tile_y = (i / (CELL_X)) % 2;
        for (size_t j = 0; j < HEIGHT; j++) {
            size_t tile_x = (j / (CELL_X)) % 2;
            if ((tile_x ^ tile_y) == 0) {
                DrawPixel(j, i, LIGHT_SHADE);
            }
            else {
                DrawPixel(j, i, DARK_SHADE);
            }
        }
    }
}
void draw_pieces(Texture* pieces)
{
    Rectangle b_pawn = {0.0f, 0.0f, (float)pieces->width/6, (float)pieces->height/2};
    Rectangle b_knight = {128.0f, 0.0f, (float)pieces->width/6, (float)pieces->height/2};
    Rectangle b_bishop = {256.0f, 0.0f, (float)pieces->width/6, (float)pieces->height/2};
    Rectangle b_rook = {384.0f, 0.0f, (float)pieces->width/6, (float)pieces->height/2};
    Rectangle b_queen = {512.0f, 0.0f, (float)pieces->width/6, (float)pieces->height/2};
    Rectangle b_king = {640.0f, 0.0f, (float)pieces->width/6, (float)pieces->height/2};

    Rectangle w_pawn = {0.0f,     128.0f, (float)pieces->width/6, (float)pieces->height/2};
    Rectangle w_knight = {128.0f, 128.0f, (float)pieces->width/6, (float)pieces->height/2};
    Rectangle w_bishop = {256.0f, 128.0f, (float)pieces->width/6, (float)pieces->height/2};
    Rectangle w_rook = {384.0f,   128.0f, (float)pieces->width/6, (float)pieces->height/2};
    Rectangle w_queen = {512.0f,  128.0f, (float)pieces->width/6, (float)pieces->height/2};
    Rectangle w_king = {640.0f,   128.0f, (float)pieces->width/6, (float)pieces->height/2};

    Rectangle texture_dest = {.x = 0.0f, .y = 0.0f, .width = 64.0f, .height = 64.0f};
    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            Vector2 pos = (Vector2) {
                .x = (j * CELL_X) - (128 * j), 
                .y = (i * CELL_Y) - (128 * i)
            };
            // if (board[i * GRID_X + j] != e_EMPTY)
            //     DrawText(TextFormat("x: %d y: %d", j, i), pos.x + (128 * j), pos.y + (128 * i), 32, RED);
            switch (board[i * GRID_X + j]) {
                case e_EMPTY: {
                    continue;
                } break;
                case w_PAWN: {
                    DrawTexturePro(*pieces, w_pawn, texture_dest, pos, 0, WHITE);
                } break;
                case w_KNIGHT: {
                    DrawTexturePro(*pieces, w_knight, texture_dest, pos, 0, WHITE);
                } break;
                case w_BISHOP: {
                    DrawTexturePro(*pieces, w_bishop, texture_dest, pos, 0, WHITE);
                } break;
                case w_ROOK: {
                    DrawTexturePro(*pieces, w_rook, texture_dest, pos, 0, WHITE);
                } break;
                case w_QUEEN: {
                    DrawTexturePro(*pieces, w_queen, texture_dest, pos, 0, WHITE);
                } break;
                case w_KING: {
                    DrawTexturePro(*pieces, w_king, texture_dest, pos, 0, WHITE);
                } break;

                case b_PAWN: {
                    DrawTexturePro(*pieces, b_pawn, texture_dest, pos, 0, WHITE);
                } break;
                case b_KNIGHT: {
                    DrawTexturePro(*pieces, b_knight, texture_dest, pos, 0, WHITE);
                } break;
                case b_BISHOP: {
                    DrawTexturePro(*pieces, b_bishop, texture_dest, pos, 0, WHITE);
                } break;
                case b_ROOK: {
                    DrawTexturePro(*pieces, b_rook, texture_dest, pos, 0, WHITE);
                } break;
                case b_QUEEN: {
                    DrawTexturePro(*pieces, b_queen, texture_dest, pos, 0, WHITE);
                } break;
                case b_KING: {
                    DrawTexturePro(*pieces, b_king, texture_dest, pos, 0, WHITE);
                } break;
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
                turn = e_BLACK;
            break;
            case 'w':
                turn = e_WHITE;
            break;
        }
    }
    else {
        init_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    }

    InitWindow(WIDTH, HEIGHT, "Chess");
    InitAudioDevice();

    Image pieces_img = LoadImageFromMemory(".png", pieces_png, pieces_png_len);
    Texture pieces = LoadTextureFromImage(pieces_img);

    Wave move_wav = LoadWaveFromMemory(".mp3", move_mp3, move_mp3_len);
    Sound move_sound = LoadSoundFromWave(move_wav);

    Wave capture_wav = LoadWaveFromMemory(".mp3", capture_mp3, capture_mp3_len);
    Sound capture_sound = LoadSoundFromWave(capture_wav);

    e_state state = NONE;

    int selected_x = 0;
    int selected_y = 0;
    e_piece selected_type = e_EMPTY;


    while (!WindowShouldClose()) { 
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse_pos = GetMousePosition();
            int x = (mouse_pos.x / CELL_X);
            int y = (mouse_pos.y / CELL_Y);

            if (state == SELECT && x == selected_x && y == selected_y) {
                state = NONE;
                selected_type = e_EMPTY;
                reset_possible_moves();
            }
            else if (state == SELECT) {
                if (possible_moves[y * GRID_X + x]) {
                    if (board[y * GRID_X + x] != e_EMPTY) {
                        PlaySound(capture_sound);
                    }
                    else {
                        PlaySound(move_sound);
                    }

                    board[y * GRID_X + x] = board[selected_y * GRID_X + selected_x];
                    board[selected_y * GRID_X + selected_x] = e_EMPTY;
                }
                selected_x = 0;
                selected_y = 0;
                state = NONE;
                selected_type = e_EMPTY;
                reset_possible_moves();
            }
            else if (board[y * GRID_X + x] != 0 && state == NONE) {
                state = SELECT;
                selected_x = x;
                selected_y = y;
                selected_type = board[y * GRID_X + x];
            }
            else {
                state = NONE;
            }
        }
        BeginDrawing();
            ClearBackground(WHITE);
            draw_board();
            draw_pieces(&pieces);

            switch (state) {
                case NONE: {

                } break;

                case SELECT: {
                    int num = find_possible_moves(selected_x, selected_y, selected_type);
                    if (num <= 0) {
                        state = NONE;
                        break;
                    }
                    for (size_t i = 0; i < GRID_Y; i++) {
                        for (size_t j = 0; j < GRID_X; j++) {
                            if (possible_moves[i * GRID_X + j]) {
                                DrawCircle((j * CELL_X) + CELL_X/2, (i * CELL_Y) + CELL_Y/2, 5, RED);
                            }
                        }
                    }
                }; break;

                default: {

                } break;
            }
        EndDrawing();
    }
    UnloadTexture(pieces);
    CloseWindow();
    return 0;
}
