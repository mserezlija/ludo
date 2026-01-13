#include <iostream>
#include "Game.h"
#include "ComputerPlayer.h"
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

Game::Game(int human_players) :curr_player(0), num_human_players(human_players) {
	srand(time(0));

	string colors[4] = { "Crvena", "Plava", "Zelena","Zuta" };
	int starts[4] = { 0, 10, 20, 30 };

	for (int i = 0; i < 4; i++) {
		if (i < num_human_players) {
			players[i] = new Player("Igrac " + to_string(i + 1), colors[i], starts[i]);
		}
		else {
			players[i] = new ComputerPlayer("Racunalo " + to_string(i + 1), colors[i], starts[i]);
		}
	}
}


Game::~Game() {
	for (int i = 0; i < 4; i++) {
		delete players[i];
	}
}

void Game::start_game() {
	cout << "Covjece, ne ljuti se!" << endl;
	cout << endl;
	cout << "Broj ljudskih igraca: " << num_human_players << endl;
	cout << "Broj racunalnih igraca: " << (4 - num_human_players) << endl;

	//demo, 12 poteza, 3 runde sa 4 igraca
	for (int r = 0; r < 4; r++) {
		cout << "_______________________________________" << endl;
		cout << "runda: " << r + 1 << endl;
		cout << endl;

		for (int i = 0; i < 4; i++) {
			players[curr_player]->play_turn();

			if (players[curr_player]->all_pieces_in_goal()) {
				cout << "pobjeda: " << players[curr_player]->get_name() << "!!!" << endl;
				return;
			}

			curr_player = (curr_player + 1) % 4;

			cout << endl;
			cout << "-----------------------------------------" << endl;
			cout << "stisni ENTER za nastaviti" << endl;
			cout << "-----------------------------------------" << endl;
			cin.get();
		}
	}

	cout << "demo gotov!" << endl;
}







