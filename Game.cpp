#include <iostream>
#include "Game.h"
#include "ComputerPlayer.h"
#include "HumanPlayer.h"
#include "GameConstants.h"
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

Game::Game(int human_players) :curr_player(0), num_human(human_players) {
	srand(time(0));

	for (int i = 0; i < GameConstants::NUM_PLAYERS; i++) {
		string name = (i < num_human) ? "igrac " + to_string(i + 1) : "komp" + to_string(i + 1);

		if (i < num_human) {
			players[i] = new HumanPlayer(name, GameConstants::PLAYER_COLORS[i], GameConstants::START_POSITIONS[i]);
		}
		else {
			players[i] = new ComputerPlayer(name, GameConstants::PLAYER_COLORS[i], GameConstants::START_POSITIONS[i]);
		}
	}
}


Game::~Game() {
	for (int i = 0; i < GameConstants::NUM_PLAYERS; i++) {
		delete players[i];
	}
}

void Game::eat(Player* current_player, Piece* moved_piece) {
	if (moved_piece->is_in_base() || moved_piece->is_in_goal()) return;

	int pos = moved_piece->get_position();
	
	for (int i = 0; i < GameConstants::NUM_PLAYERS; i++) {
		if (players[i] == current_player) continue;

		for (int j = 0; j < GameConstants::NUM_PIECES_PER_PLAYER; j++) {
			Piece* enemy = players[i]->get_piece(j);
			if (enemy && enemy->get_position() == pos && !enemy->is_in_base() && !enemy->is_in_goal()) {
				enemy->return_to_base();
				cout << players[i]->get_color() << " je pokosen" << endl;
			}
		}
	}
}

void Game::start_game() {
	cout << "Covjece, ne ljuti se!" << endl;
	cout << endl;
	cout << "Broj ljudskih igraca: " << num_human << endl;
	cout << "Broj racunalnih igraca: " << (4-num_human) << endl;

	bool game_over = false;

	while (!game_over) {
		players[curr_player]->play_turn();

		if (players[curr_player]->all_pieces_in_goal()) {
			cout << "pobjeda " << players[curr_player]->get_color() << "!!!!!!!!!!!" << endl;
			game_over = true;
		}
		curr_player = (curr_player + 1) % GameConstants::NUM_PLAYERS;
	}
}







