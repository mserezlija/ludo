#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include <string>
#include "GameConstants.h"

using namespace std;

class Game;

class Graphics {
private:
    Game* game;
    Color player_colors[4] = { RED, DARKBLUE, DARKGREEN, ORANGE };
    int last_dice = 0;

    Vector2 board_positions[BOARD_SIZE];
    Vector2 base_positions[4][4];
    Vector2 home_positions[4][4];

    string message = "";
    int selected_piece = -1;
    bool waiting_for_roll = false;
    bool waiting_for_piece = false;
    bool waiting_for_new_or_move = false;

    void init_positions();
    void draw_board();
    //void draw_bases();
    void draw_homes();
    void draw_pieces();
    void draw_dice();
    void draw_message();
    void draw_buttons();

    int get_clicked_piece(int player_idx);
    bool is_point_in_circle(Vector2 point, Vector2 center, float radius);

public:
    Graphics(Game* g);
    ~Graphics();
    
    void set_dice(int dice_val);
    void set_game(Game* g);

    int wait_for_roll();
    int wait_for_piece_selection(int curr_player, int dice);
    bool wait_for_new_or_move();
    int wait_for_player_cnt();
    int wait_for_color_sel(bool taken[4]);

    void set_message(const string& msg);

    void update();
    bool should_close();
};

#endif