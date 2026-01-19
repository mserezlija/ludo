#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "GameConstants.h"

class Game {
private:
	Player* players[GameConstants::NUM_PLAYERS];
	int curr_player;
	int num_human;

	void eat(Player* current_player, Piece* moved_piece);
public:
	Game(int human_players = 1);
	~Game();

	void start_game();
};

#endif
