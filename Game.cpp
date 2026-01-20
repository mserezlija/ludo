#include <iostream>
#include "Game.h"
#include "ComputerPlayer.h"
#include "HumanPlayer.h"
#include "GameConstants.h"
#include "Graphics.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

Game::Game(int human_players) :curr_player(0), num_human(human_players) {
	srand(time(0));

	board = new Board(this);

	bool taken_colors[NUM_PLAYERS] = { false };

	for (int i = 0; i < NUM_PLAYERS; i++) {
		string name = (i < num_human) ? "igrac " + to_string(i + 1) : "komp" + to_string(i + 1);

		string color;
		int start_pos = START_POSITIONS[i];

		if (i < num_human) {
			color = pick_color(i + 1, taken_colors);

			for (int j = 0; j < NUM_PLAYERS; j++) {
				if (PLAYER_COLORS[j] == color) {
					start_pos = START_POSITIONS[j];
					break;
				}
			}
		}
		else {
			for (int j = 0; j < NUM_PLAYERS; j++) {
				if (!taken_colors[j]) {
					taken_colors[j] = true;
					color = PLAYER_COLORS[j];
					start_pos = START_POSITIONS[j];
					break;
				}
			}
		}

		if (i < num_human) {
			players[i] = new HumanPlayer(name, color, start_pos);
		}
		else {
			players[i] = new ComputerPlayer(name, color, start_pos);
		}
	}
}

string Game::pick_color(int player_n, bool taken_colors[]) {
	cout << "odaberi boju: ";
	for (int i = 0; i < NUM_PLAYERS; i++) {
		if (!taken_colors[i]) {
			cout << "  " << (i + 1) << " - " << PLAYER_COLORS[i] << endl;
		}
	}

	int picked;
	cin >> picked;
	cin.ignore();
	picked--;

	if (picked >= 0 && picked < NUM_PLAYERS && !taken_colors[picked]) {
		taken_colors[picked] = true;
		return PLAYER_COLORS[picked];
	}
	cout << "neispravan unos, automatski dodiljenja boja" << endl;
	for (int i = 0; i < NUM_PLAYERS; i++) {
		if (!taken_colors[i]) {
			taken_colors[i] = true;
			return PLAYER_COLORS[i];
		}
	}
}

Game::~Game() {
	for (int i = 0; i < NUM_PLAYERS; i++) {
		delete players[i];
	}
	delete board;
}

void Game::update_dice(int dice_val) {
    if (graphics) {
        graphics->set_dice(dice_val);
        graphics->update();
    }
}

void Game::set_graphics(Graphics* g) { graphics = g; }

Player* Game::get_player(int index) {
	if (index >= 0 && index < NUM_PLAYERS) {
		return players[index];
	}
	return nullptr;
}


void Game::start_game() {
	cout << "Covjece, ne ljuti se!" << endl;
	cout << endl;
	cout << "Broj ljudskih igraca: " << num_human << endl;
	cout << "Broj racunalnih igraca: " << (4 - num_human) << endl;

	if (graphics) { graphics->update(); }

	bool game_over = false;

	while (!game_over) {
		if (graphics) { graphics->update(); }

		players[curr_player]->play_turn(board);
		if (graphics) { graphics->update(); }


		if (players[curr_player]->all_pieces_in_goal()) {
			cout << "pobjeda " << players[curr_player]->get_color() << "!!!!!!!!!!!" << endl;
			game_over = true;
		}
		curr_player = (curr_player + 1) % NUM_PLAYERS;
	}

	if (graphics) {
		while (!graphics->should_close()) {
			graphics->update();
		}
	}
}

