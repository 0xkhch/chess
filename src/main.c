#include "common.h"
#include "board.h"
#include "draw.h"
#include "control.h"

// textures
#include "pieces.h"
#include "move.h"
#include "capture.h"
global_state_t global = {};

//TODO: bitboards
//TODO: Threatmaps and checks
//TODO: en passants
//TODO: castling 
//TODO: promotion
//TODO: start and end screen

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
        // init_board("K5Nk/8/8/8/8/8/PPPPP3/k6K");
        init_board("P2r2Nk/8/8/8/3kQ3/8/3R4/kB5b");
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
        BeginDrawing();
            draw_board(&pieces);
            ClearBackground(WHITE);
            switch (global.state) {
                case NONE: {
                    global.selected_moves = 0;
                } break;
                case SELECT: {
                    find_selected_moves(&global);
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
