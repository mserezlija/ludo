#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "GameConstants.h"
#include <vector>
#include <string>

class Game {
private:
	Player* players[GameConstants::NUM_PLAYERS];
	int curr_player;
	int num_human;

	string pick_color(int player_n, bool taken_colors[]);

	void eat(Player* current_player, Piece* moved_piece);
public:
	Game(int human_players = 1);
	~Game();

	void start_game();
};

#endif
