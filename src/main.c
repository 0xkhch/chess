#include "common.h"
#include "board.h"
#include "draw.h"
#include "control.h"

// textures
#include "pieces.h"
#include "move.h"
#include "capture.h"
global_state_t global = {};

// en passant checks
// pin checks
//TODO: castling 
//TODO: promotion
//TODO: start and end screen

bool save_board()
{
    FILE* f = fopen("saved.txt", "w+");
    if (f == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return false;
    }

    size_t i = 0;
    while (i < GRID_SIZE) {
        if (i % GRID_X == 0 && i != 0) {
            putc('/', f);
        }
        if (board[i] == e_EMPTY) {
            size_t cnt = 0;
            char digits[] = {'1', '2', '3', '4', '5', '6', '7', '8'};
            while (board[++i] == e_EMPTY) {
                if (i % 8 == 0) {
                    break;
                }
                cnt++;
            }
            fputc(digits[cnt], f);
            continue;
        }
        fputc(enum_to_char(board[i]), f);
        i++;
    }
    fputc(' ', f);
    fputc(global.turn ? 'b' : 'w', f);
    printf("INFO: saved board.\n");
    fclose(f);
    return true;
}


int main(int argc, char** argv)
{
    if (argc > 3) {
        fprintf(stderr, "%s\n", "Too many arguments...");
        return 1;
    }
    else if (argc == 3) {
        load_board(argv[argc - 2]);
        set_turn(*argv[argc - 1]);
    }
    else {
        load_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    }
    InitWindow(WIDTH, HEIGHT, "Chess");
    InitAudioDevice();

    Image pieces_img = LoadImageFromMemory(".png", pieces_png, pieces_png_len);
    Wave move_wav    = LoadWaveFromMemory(".mp3", move_mp3, move_mp3_len);
    Wave capture_wav    = LoadWaveFromMemory(".mp3", capture_mp3, capture_mp3_len);
    Texture2D pieces   = LoadTextureFromImage(pieces_img);
    Sound move_sound = LoadSoundFromWave(move_wav);
    Sound capture_sound = LoadSoundFromWave(capture_wav);

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
            control(&move_sound, &capture_sound, x, y);
        }

#ifdef DEBUG
        if (IsKeyPressed(KEY_S)) {
            save_board();
        }
        if (IsKeyPressed(KEY_L)) {
            FILE* f = fopen("saved.txt", "r");
            if (f == NULL) {
                fprintf(stderr, "Failed to open file\n");
                return false;
            }
            char buffer[64] = {};
            fread(buffer, sizeof(char), 64, f);
            load_board(buffer);

            global.selected_moves = 0;
            global.state = s_NONE;
            global.prev.type = e_EMPTY;

            printf("INFO: loaded board.\n");
            fclose(f);
        }
#endif /* ifdef DEBUG */

        check();
        BeginDrawing();
            draw_board(&pieces);
            ClearBackground(WHITE);
            switch (global.state) {
                case s_NONE: {
                    global.selected_moves = 0;
                } break;
                case s_SELECT: {
                    find_selected_moves();
                }; break;
                default: {
                } break;
            }

#ifdef DEBUG
            DrawFPS(0, 0);
#endif
        EndDrawing();
    }
    UnloadTexture(pieces);
    CloseWindow();
    return 0;
}
