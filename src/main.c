#include "common.h"
#include "board.h"
#include "draw.h"
#include "control.h"

// textures
#include "pieces.h"
#include "move.h"
#include "capture.h"
global_state_t global = {0};
global_state_t previous = {0};


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

bool load_file(char* src)
{
    FILE* f = fopen(src, "r");
    if (f == NULL) {
        fprintf(stderr, "INFO: Failed to open file\n");
        return false;
    }
    char buffer[64] = {};
    fread(buffer, sizeof(char), 64, f);
    load_board(buffer);
    global.screen_state = PLAY;
    global.selected_moves = 0;
    global.heatmap = 0;
    global.state = s_NONE;
    global.prev.type = e_EMPTY;

    check();
    printf("INFO: loaded board.\n");
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
    check();
    while (!WindowShouldClose()) { 
        Vector2 mouse_pos = GetMousePosition();
        x = (mouse_pos.x / CELL_X);
        y = (mouse_pos.y / CELL_Y);
        global.mouse_pos_x = x;
        global.mouse_pos_y = y;


        if (IsKeyPressed(KEY_S) && global.screen_state == PLAY) {
            save_board();
        }
        if (IsKeyPressed(KEY_L)) {
            load_file("saved.txt");
        }
        if (IsFileDropped()) {
            FilePathList files = LoadDroppedFiles();
            load_file(files.paths[files.count - 1]); // load last dropped file
            UnloadDroppedFiles(files);
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z) && global.screen_state == PLAY && global.moves_counter != 0) {
            copy_board(board, last_board);
            copy_state(&global, &previous);
            clear_selection();
        }

        BeginDrawing();
            ClearBackground(WHITE);
            switch (global.screen_state) {
            case INTRO:
#ifdef DEBUG
    global.screen_state = MENU;
#endif /* ifdef DEBUG */
                draw_intro();
                break;
            case MENU:
                draw_menu(&pieces);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && global.inside_option) {
                    global.screen_state = PLAY;
                }
                break;
            case PLAY:
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    control(&move_sound, &capture_sound, x, y);
                }
                draw_board(&pieces);
                break;
            case PROMO:
                draw_promo(&pieces);
                if (global.mouse_pos_x >= 2 && global.mouse_pos_x < 6 && global.mouse_pos_y == 4) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (global.mouse_pos_x == 2) {
                            board[global.promo_y * GRID_X + global.promo_x] = global.turn == t_WHITE ? w_QUEEN: b_QUEEN;
                        }
                        if (global.mouse_pos_x == 3) {
                            board[global.promo_y * GRID_X + global.promo_x] = global.turn == t_WHITE ? w_ROOK: b_ROOK;
                        }
                        if (global.mouse_pos_x == 4) {
                            board[global.promo_y * GRID_X + global.promo_x] = global.turn == t_WHITE ? w_BISHOP: b_BISHOP;
                        }
                        if (global.mouse_pos_x == 5) {
                            board[global.promo_y * GRID_X + global.promo_x] = global.turn == t_WHITE ? w_KNIGHT: b_KNIGHT;
                        }
                        global.screen_state = PLAY;
                        global.turn = !global.turn;
                        check();
                    }
                }
                break;
            case END:
                draw_end(&pieces);
                break;
            case ERROR:
                draw_error(&pieces);
                break;
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
