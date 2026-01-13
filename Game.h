#ifndef GAME_H
#define GAME_H

#include "Player.h"

class Game {
private:
	Player* players[4];
	int curr_player;
	int num_human_players;
public:
	Game(int human_players = 1);
	~Game();

	void start_game();
};

#endif
