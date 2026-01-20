#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "GameConstants.h"
#include "Board.h"
#include <vector>
#include <string>

class Graphics;

class Game {
private:
	Player* players[NUM_PLAYERS];
	Board* board;
	Graphics* graphics = nullptr;

	int curr_player;
	int num_human;
	int curr_dice = 0;

	string pick_color(int player_n, bool taken_colors[]);

public:
	Game(int human_players = 1, Graphics* graph = nullptr);
	~Game();
	
	void set_curr_dice(int dice_val);
	int get_curr_dice() const;
	void update_dice(int dice_val);

	void start_game();
	Player* get_player(int index);
	Player* get_curr_player();

	void connect_graphics_to_players();
	void set_graphics(Graphics* g);
	Graphics* get_graphics();

};

#endif
