#include "Graphics.h"
#include "Game.h"
#include "Player.h"
#include "Piece.h"
#include <string>
#include <cmath>

const int WINDOW_SIZE = 900;
const int PIECE_RADIUS = 16;

Graphics::Graphics(Game* g) : game(g) {
    InitWindow(WINDOW_SIZE, WINDOW_SIZE, "Covjece ne ljuti se!");
    SetTargetFPS(60);
    init_positions();
}

Graphics::~Graphics() {
    CloseWindow();
}

void Graphics::init_positions() {
    int cx = WINDOW_SIZE / 2;
    int cy = WINDOW_SIZE / 2;

    for (int i = 0; i < BOARD_SIZE; i++) {
        float angle = (i * 9.0f - 90) * 3.14159f / 180.0f;
        float radius = 320;
        board_positions[i].x = cx + radius * cos(angle);
        board_positions[i].y = cy + radius * sin(angle);
    }

    // Baze u kutevima
    base_positions[0][0] = { 50, 50 };
    base_positions[0][1] = { 100, 50 };
    base_positions[0][2] = { 50, 100 };
    base_positions[0][3] = { 100, 100 };

    base_positions[1][0] = { 800, 50 };
    base_positions[1][1] = { 850, 50 };
    base_positions[1][2] = { 800, 100 };
    base_positions[1][3] = { 850, 100 };

    base_positions[2][0] = { 800, 800 };
    base_positions[2][1] = { 850, 800 };
    base_positions[2][2] = { 800, 850 };
    base_positions[2][3] = { 850, 850 };

    base_positions[3][0] = { 50, 800 };
    base_positions[3][1] = { 100, 800 };
    base_positions[3][2] = { 50, 850 };
    base_positions[3][3] = { 100, 850 };

    // Kucice
    for (int p = 0; p < 4; p++) {
        float angle = (p * 90 - 90) * 3.14159f / 180.0f;
        for (int i = 0; i < 4; i++) {
            float dist = 80 + i * 50;
            home_positions[p][i].x = cx + dist * cos(angle);
            home_positions[p][i].y = cy + dist * sin(angle);
        }
    }
}

void Graphics::draw_board() {
    ClearBackground(Color{ 40, 40, 40, 255 });

    for (int i = 0; i < BOARD_SIZE; i++) {
        Color c = LIGHTGRAY;
        if (i == 0) c = RED;
        else if (i == 10) c = BLUE;
        else if (i == 20) c = GREEN;
        else if (i == 30) c = ORANGE;

        DrawCircle((int)board_positions[i].x + 2, (int)board_positions[i].y + 2, 18, Fade(BLACK, 0.3f));
        DrawCircle((int)board_positions[i].x, (int)board_positions[i].y, 18, c);
        DrawCircleLines((int)board_positions[i].x, (int)board_positions[i].y, 18, BLACK);
    }
}

void Graphics::draw_bases() {
    for (int p = 0; p < 4; p++) {
        for (int i = 0; i < 4; i++) {
            DrawCircle((int)base_positions[p][i].x, (int)base_positions[p][i].y, 14, Fade(player_colors[p], 0.3f));
        }
    }
}

void Graphics::draw_homes() {
    for (int p = 0; p < 4; p++) {
        for (int i = 0; i < 4; i++) {
            DrawCircle((int)home_positions[p][i].x + 2, (int)home_positions[p][i].y + 2, 16, Fade(BLACK, 0.3f));
            DrawCircle((int)home_positions[p][i].x, (int)home_positions[p][i].y, 16, Fade(player_colors[p], 0.4f));
        }
    }
}

void Graphics::draw_pieces() {
    if (!game) return;

    for (int p = 0; p < NUM_PLAYERS; p++) {
        Player* player = game->get_player(p);
        if (!player) continue;

        int color_idx = 0;
        string col = player->get_color();
        if (col == "Crvena") color_idx = 0;
        else if (col == "Plava") color_idx = 1;
        else if (col == "Zelena") color_idx = 2;
        else if (col == "Zuta") color_idx = 3;

        int base_idx = player->get_start_pos() / 10;

        for (int i = 0; i < NUM_PIECES_PER_PLAYER; i++) {
            Piece* piece = player->get_piece(i);
            if (!piece) continue;

            float x = 0, y = 0;
            bool draw = true;

            if (piece->is_in_base()) {
                x = base_positions[base_idx][i].x;
                y = base_positions[base_idx][i].y;
            }
            else if (piece->is_in_goal()) {
                // U cilju = zadnje polje kucice (pozicija 4)
                x = home_positions[base_idx][3].x;
                y = home_positions[base_idx][3].y;
            }
            else if (piece->is_in_home()) {
                int home_idx = piece->get_steps_taken() - BOARD_SIZE - 1;
                if (home_idx >= 0 && home_idx < 4) {
                    x = home_positions[base_idx][home_idx].x;
                    y = home_positions[base_idx][home_idx].y;
                }
                else draw = false;
            }
            else {
                int pos = piece->get_position();
                if (pos >= 0 && pos < BOARD_SIZE) {
                    x = board_positions[pos].x;
                    y = board_positions[pos].y;
                }
                else draw = false;
            }

            if (draw) {
                DrawCircle((int)x + 2, (int)y + 2, PIECE_RADIUS, Fade(BLACK, 0.4f));
                DrawCircle((int)x, (int)y, PIECE_RADIUS, player_colors[color_idx]);
                DrawCircleLines((int)x, (int)y, PIECE_RADIUS, BLACK);
                DrawCircle((int)x - 3, (int)y - 3, 5, Fade(LIGHTGRAY, 0.5f));
            }
        }
    }
}

void Graphics::set_dice(int dice_val) {
    last_dice = dice_val;
}

void Graphics::draw_dice() {
    int cx = WINDOW_SIZE / 2;
    int cy = WINDOW_SIZE / 2;

    // Bijeli kvadrat
    DrawRectangle(cx - 35, cy - 35, 70, 70, WHITE);
    DrawRectangleLines(cx - 35, cy - 35, 70, 70, BLACK);

    // Tocke ovisno o broju
    Color dot = BLACK;
    int r = 6;

    if (last_dice == 1 || last_dice == 3 || last_dice == 5) {
        DrawCircle(cx, cy, r, dot);  // sredina
    }
    if (last_dice >= 2) {
        DrawCircle(cx - 15, cy - 15, r, dot);  // gore lijevo
        DrawCircle(cx + 15, cy + 15, r, dot);  // dolje desno
    }
    if (last_dice >= 4) {
        DrawCircle(cx + 15, cy - 15, r, dot);  // gore desno
        DrawCircle(cx - 15, cy + 15, r, dot);  // dolje lijevo
    }
    if (last_dice == 6) {
        DrawCircle(cx - 15, cy, r, dot);  // lijevo sredina
        DrawCircle(cx + 15, cy, r, dot);  // desno sredina
    }
}


void Graphics::update() {
    if (WindowShouldClose()) return;

    BeginDrawing();
    draw_board();
    draw_bases();
    draw_homes();
    draw_pieces();
    draw_dice();
    DrawText("COVJECE NE LJUTI SE", 320, 10, 26, WHITE);
    EndDrawing();
}

bool Graphics::should_close() {
    return WindowShouldClose();
}