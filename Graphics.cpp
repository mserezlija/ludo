#include "Graphics.h"
#include "Game.h"
#include "Player.h"
#include "Piece.h"
#include <string>
#include <cmath>

const int WINDOW_SIZE = 900;
const int PIECE_RADIUS = 16;

Graphics::Graphics(Game* g) : game(g) {
    InitWindow(WINDOW_SIZE, WINDOW_SIZE, "Covjece, ne ljuti se!");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

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

void Graphics::draw_homes() {
    for (int p = 0; p < 4; p++) {
        for (int i = 0; i < 4; i++) {
            DrawCircle((int)home_positions[p][i].x + 2, (int)home_positions[p][i].y + 2, 16, Fade(BLACK, 0.3f));
            DrawCircle((int)home_positions[p][i].x, (int)home_positions[p][i].y, 16, Fade(player_colors[p], 0.4f));
        }
    }
}

void Graphics::draw_pieces() {
    if (!game) {
        return;
    }


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

void Graphics::set_game(Game* g) { game = g; }

void Graphics::draw_dice() {
    int cx = WINDOW_SIZE / 2;
    int cy = WINDOW_SIZE / 2;
    int size = 60;
    int half = size / 2;
    int r = 7;

    //kockice prema igracu koji je na redu
    Color dice_color = WHITE;
    if (game) {
        Player* curr = game->get_curr_player();
        if (curr) {
            string col = curr->get_color();
            if (col == "Crvena") dice_color = RED;
            else if (col == "Plava") dice_color = BLUE;
            else if (col == "Zelena") dice_color = GREEN;
            else if (col == "Zuta") dice_color = ORANGE;
        }
    }

    DrawRectangle(cx - half, cy - half, size, size, dice_color);
    DrawRectangleLines(cx - half, cy - half, size, size, BLACK);

    int dice = last_dice;
    //if (game) dice = game->get_curr_dice();

    int left = cx - 18;
    int right = cx + 18;
    int top = cy - 18;
    int bottom = cy + 18;

    Color dot = BLACK;

    // 1 - sredina
    if (dice == 1) {
        DrawCircle(cx, cy, r, dot);
    }
    // 2 - dijagonala
    else if (dice == 2) {
        DrawCircle(left, top, r, dot);
        DrawCircle(right, bottom, r, dot);
    }
    // 3 - dijagonala + sredina
    else if (dice == 3) {
        DrawCircle(left, top, r, dot);
        DrawCircle(cx, cy, r, dot);
        DrawCircle(right, bottom, r, dot);
    }
    // 4 - sva 4 kuta
    else if (dice == 4) {
        DrawCircle(left, top, r, dot);
        DrawCircle(right, top, r, dot);
        DrawCircle(left, bottom, r, dot);
        DrawCircle(right, bottom, r, dot);
    }
    // 5 - sva 4 kuta + sredina
    else if (dice == 5) {
        DrawCircle(left, top, r, dot);
        DrawCircle(right, top, r, dot);
        DrawCircle(cx, cy, r, dot);
        DrawCircle(left, bottom, r, dot);
        DrawCircle(right, bottom, r, dot);
    }
    // 6 - 2 stupca po 3
    else if (dice == 6) {
        DrawCircle(left, top, r, dot);
        DrawCircle(left, cy, r, dot);
        DrawCircle(left, bottom, r, dot);
        DrawCircle(right, top, r, dot);
        DrawCircle(right, cy, r, dot);
        DrawCircle(right, bottom, r, dot);
    }
}

void Graphics::set_message(const std::string& msg) { message = msg; }

void Graphics::draw_message() {
    if (!message.empty()) {
        DrawRectangle(200, 820, 500, 40, Fade(BLACK, 0.7f));
        DrawText(message.c_str(), 220, 830, 20, WHITE);
    }
}

void Graphics::draw_buttons() {
    if (waiting_for_roll) {
        DrawRectangle(350, 820, 200, 50, GREEN);
        DrawRectangleLines(350, 820, 200, 50, BLACK);
        DrawText("BACI KOCKICU", 375, 835, 20, BLACK);
    }

    if (waiting_for_new_or_move) {
        DrawRectangle(200, 750, 200, 50, BLUE);
        DrawText("NOVA FIGURA", 225, 765, 20, WHITE);

        DrawRectangle(500, 750, 200, 50, ORANGE);
        DrawText("POMAKNI", 545, 765, 20, WHITE);
    }
}

bool Graphics::is_point_in_circle(Vector2 point, Vector2 center, float radius) {
    float dx = point.x - center.x;
    float dy = point.y - center.y;
    return (dx * dx + dy * dy) <= (radius * radius);
}

int Graphics::get_clicked_piece(int player_idx) {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return -1;

    Vector2 mouse = GetMousePosition();
    Player* player = game->get_player(player_idx);
    if (!player) return -1;

    int base_idx = player->get_start_pos() / 10;

    for (int i = 0; i < NUM_PIECES_PER_PLAYER; i++) {
        Piece* piece = player->get_piece(i);
        if (!piece) continue;

        Vector2 pos = { 0, 0 };

        if (piece->is_in_base()) {
            pos = base_positions[base_idx][i];
        }
        else if (!piece->is_in_goal()) {
            if (piece->is_in_home()) {
                int home_idx = piece->get_steps_taken() - BOARD_SIZE - 1;
                if (home_idx >= 0 && home_idx < 4) {
                    pos = home_positions[base_idx][home_idx];
                }
            }
            else {
                int p = piece->get_position();
                if (p >= 0 && p < BOARD_SIZE) {
                    pos = board_positions[p];
                }
            }
        }

        if (is_point_in_circle(mouse, pos, PIECE_RADIUS + 5)) {
            return i;
        }
    }
    return -1;
}

int Graphics::wait_for_roll() {
    waiting_for_roll = true;

    while (!WindowShouldClose()) {
        update();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            if (mouse.x >= 350 && mouse.x <= 550 && mouse.y >= 820 && mouse.y <= 870) {
                waiting_for_roll = false;
                int result = (rand() % MAX_DICE_VALUE) + 1;
                return result;
            }
        }

        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
            waiting_for_roll = false;
            int result = (rand() % MAX_DICE_VALUE) + 1;
            return result;
        }
    }

    waiting_for_roll = false;
    return 0;
}

int Graphics::wait_for_piece_selection(int current_player, int dice) {
    if (!game) return -1;
    waiting_for_piece = true;
    set_message("Klikni na figuru za pomaknuti je na ploci");

    Player* player = game->get_player(current_player);

    if (!player) {
        waiting_for_piece = false;
        set_message("");
        return -1;
    }

    while (!WindowShouldClose()) {
        update();

        int clicked = get_clicked_piece(current_player);
        if (clicked >= 0) {
            Piece* piece = player->get_piece(clicked);
            if (piece && !piece->is_in_base() && !piece->is_in_goal() && piece->can_move(dice)) {
                waiting_for_piece = false;
                set_message("");
                return clicked;
            }
        }
    }
    waiting_for_piece = false;
    return -1;
}

bool Graphics::wait_for_new_or_move() {
    waiting_for_new_or_move = true;
    set_message("Dobio si 6! Odaberi akciju.");

    while (!WindowShouldClose()) {
        update();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();

            if (mouse.x >= 200 && mouse.x <= 400 && mouse.y >= 750 && mouse.y <= 800) {
                waiting_for_new_or_move = false;
                set_message("");
                return true;
            }

            if (mouse.x >= 500 && mouse.x <= 700 && mouse.y >= 750 && mouse.y <= 800) {
                waiting_for_new_or_move = false;
                set_message("");
                return false;
            }
        }
    }

    waiting_for_new_or_move = false;
    return false;
}

int Graphics::wait_for_player_cnt() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        DrawText("COVJECE, NE LJUTI SE", 300, 100, 30, WHITE);
        DrawText("Odaberi broj ljudskih igraca:", 270, 250, 24, WHITE);


        // Gumbi 0-4
        for (int i = 1; i <= 4; i++) {
            int x = 270 + (i - 1) * 100;
            DrawRectangle(x, 350, 80, 80, WHITE);
            DrawRectangleLines(x, 350, 80, 80, BLACK);
            char num[2] = { (char)('0' + i), '\0' };
            DrawText(num, x + 30, 370, 40, BLACK);
        }

        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i <= 4; i++) {
                int x = 270 + (i - 1) * 100;
                if (mouse.x >= x && mouse.x <= x + 80 && mouse.y >= 350 && mouse.y <= 430) {
                    return i;
                }
            }
        }
    }
    return 1;
}

int Graphics::wait_for_color_sel(bool taken[4]) {
    const char* color_names[4] = { "CRVENA", "PLAVA", "ZELENA", "ZUTA" };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        DrawText("Odaberi boju:", 350, 200, 30, WHITE);

        for (int i = 0; i < 4; i++) {
            if (taken[i]) continue;

            int x = 150 + i * 180;
            DrawCircle(x + 60, 400, 50, player_colors[i]);
            DrawCircleLines(x + 60, 400, 50, BLACK);
            DrawText(color_names[i], x + 20, 480, 20, WHITE);
        }

        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < 4; i++) {
                if (taken[i]) continue;
                int x = 150 + i * 180;
                Vector2 center = { (float)(x + 60), 400 };
                if (is_point_in_circle(mouse, center, 50)) {
                    return i;
                }
            }
        }
    }
    return 0;
}


void Graphics::update() {
    if (WindowShouldClose()) return;

    BeginDrawing();
    draw_board();
    draw_homes();
    draw_pieces();
    draw_dice();
    draw_buttons();
    draw_message();
    DrawText("COVJECE, NE LJUTI SE", 320, 10, 26, WHITE);
    EndDrawing();
}

bool Graphics::should_close() {
    return WindowShouldClose();
}