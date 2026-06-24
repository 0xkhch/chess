#include "common.h"
#include "raylib.h"
#include "board.h"

#include "pieces.h"
#include "move.h"
#include "capture.h"

typedef struct global_state {
    int selected_x;
    int selected_y;
    e_piece selected_type;
    e_state state;
    bool turn;
} global_state_t;

//TODO: make these dynamic ?
#define LIGHT_SHADE (Color){.r = 0xFF, .g = 0xCF, .b = 0x9F, .a = 0xFF}
#define DARK_SHADE (Color){.r = 0xD2, .g = 0x8C, .b = 0x45, .a = 0xFF}

#define t_WHITE false
#define t_BLACK true


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
void draw_pieces(Texture2D* pieces)
{
#define TEXTURE_WIDTH_COUNT (6)
#define TEXTURE_HEIGHT_COUNT (2)
    Rectangle b_pawn   = {0.0f, 0.0f,   (float)pieces->width/TEXTURE_WIDTH_COUNT, (float)pieces->height/TEXTURE_HEIGHT_COUNT};
    Rectangle b_knight = {128.0f, 0.0f, (float)pieces->width/TEXTURE_WIDTH_COUNT, (float)pieces->height/TEXTURE_HEIGHT_COUNT};
    Rectangle b_bishop = {256.0f, 0.0f, (float)pieces->width/TEXTURE_WIDTH_COUNT, (float)pieces->height/TEXTURE_HEIGHT_COUNT};
    Rectangle b_rook   = {384.0f, 0.0f, (float)pieces->width/TEXTURE_WIDTH_COUNT, (float)pieces->height/TEXTURE_HEIGHT_COUNT};
    Rectangle b_queen  = {512.0f, 0.0f, (float)pieces->width/TEXTURE_WIDTH_COUNT, (float)pieces->height/TEXTURE_HEIGHT_COUNT};
    Rectangle b_king   = {640.0f, 0.0f, (float)pieces->width/TEXTURE_WIDTH_COUNT, (float)pieces->height/TEXTURE_HEIGHT_COUNT};

    Rectangle w_pawn   = {0.0f,   128.0f, (float)pieces->width/TEXTURE_WIDTH_COUNT, (float)pieces->height/TEXTURE_HEIGHT_COUNT};
    Rectangle w_knight = {128.0f, 128.0f, (float)pieces->width/TEXTURE_WIDTH_COUNT, (float)pieces->height/TEXTURE_HEIGHT_COUNT};
    Rectangle w_bishop = {256.0f, 128.0f, (float)pieces->width/TEXTURE_WIDTH_COUNT, (float)pieces->height/TEXTURE_HEIGHT_COUNT};
    Rectangle w_rook   = {384.0f, 128.0f, (float)pieces->width/TEXTURE_WIDTH_COUNT, (float)pieces->height/TEXTURE_HEIGHT_COUNT};
    Rectangle w_queen  = {512.0f, 128.0f, (float)pieces->width/TEXTURE_WIDTH_COUNT, (float)pieces->height/TEXTURE_HEIGHT_COUNT};
    Rectangle w_king   = {640.0f, 128.0f, (float)pieces->width/TEXTURE_WIDTH_COUNT, (float)pieces->height/TEXTURE_HEIGHT_COUNT};

    Rectangle texture_dest = {.x = 0.0f, .y = 0.0f, .width = 64.0f, .height = 64.0f};
    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            Vector2 pos = (Vector2) {
                .x = (j * CELL_X) - (128 * j), 
                .y = (i * CELL_Y) - (128 * i)
            };
#ifdef DEBUG
            if (board[i * GRID_X + j] != e_EMPTY)
                DrawText(TextFormat("x: %d y: %d", j, i), pos.x + (128 * j), pos.y + (128 * i), 24, RED);
#endif // DEBUG
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
    global_state_t global = {};
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

    while (!WindowShouldClose()) { 
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse_pos = GetMousePosition();
            int x = (mouse_pos.x / CELL_X);
            int y = (mouse_pos.y / CELL_Y);

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
                        reset_possible_moves();
                        break;
                    }
                    if ((board[y * GRID_X + x] > 0 && global.turn == t_WHITE) 
                        || (board[y * GRID_X + x] < 0 && global.turn == t_BLACK)) {
                        global.state = SELECT;
                        global.selected_x = x;
                        global.selected_y = y;
                        global.selected_type = board[y * GRID_X + x];
                        reset_possible_moves();
                    }
                    else if (!possible_moves[y * GRID_X + x] && ((board[y * GRID_X + x] < 0 && global.turn == t_WHITE) 
                            || (board[y * GRID_X + x] > 0 && global.turn == t_BLACK))) {
                        global.state = NONE;
                        global.selected_type = e_EMPTY;
                        global.selected_x = 0;
                        global.selected_y = 0;
                        reset_possible_moves();
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
                        reset_possible_moves();
                    }
#endif /* ifdef DEBUG */
                } break;
            }
        }
        BeginDrawing();
            ClearBackground(WHITE);
            draw_board();
            draw_pieces(&pieces);
            switch (global.state) {
                case NONE: {
                } break;
                case SELECT: {
                    // TODO: compute this once mayhaps
                    int num = find_possible_moves(global.selected_x, global.selected_y, global.selected_type);
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
