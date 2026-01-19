#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "GameConstants.h"
#include "Board.h"
#include <vector>
#include <string>

class Game {
private:
	Player* players[NUM_PLAYERS];

	Board* board;

	int curr_player;
	int num_human;

	string pick_color(int player_n, bool taken_colors[]);

public:
	Game(int human_players = 1);
	~Game();

	void start_game();
	Player* get_player(int index);
};

#endif
