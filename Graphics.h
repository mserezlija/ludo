#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include "GameConstants.h"
class Game;

class Graphics {
private:
    Game* game;
    Color player_colors[4] = { RED, BLUE, GREEN, ORANGE };
    int last_dice = 0;

    Vector2 board_positions[BOARD_SIZE];
    Vector2 base_positions[4][4];
    Vector2 home_positions[4][4];

    void init_positions();
    void draw_board();
    void draw_bases();
    void draw_homes();
    void draw_pieces();
    void draw_dice();

public:
    Graphics(Game* g);
    ~Graphics();
    
    void set_dice(int dice_val);

    void update();
    bool should_close();
};

#endif