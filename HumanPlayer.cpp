#include "HumanPlayer.h"
#include <iostream>
#include <cstdlib>

using namespace std;

HumanPlayer::HumanPlayer(const string& name, const string& color, int start_pos) : Player(name, color, start_pos) {};

int HumanPlayer::roll_dice() {
	cout << "stisni enter za bacit kockicu" << endl;
	cin.get();
	return (rand() % MAX_DICE_VALUE) + 1;
}

int HumanPlayer::choose_piece(int dice) {
	cout << "dostupne figure: " << endl;

	for (int i = 0; i < NUM_PIECES_PER_PLAYER; i++) {
		if (!pieces[i].is_in_base() && !pieces[i].is_in_goal() && pieces[i].can_move(dice)) {
			cout << "figura " << (i + 1) << " - Pozicija: " << pieces[i].get_position();
			if (pieces[i].is_in_home()) {
				cout << " (kucica)";
			}
			cout << endl;
		}
	}

	while (true) {
		cout << "odaberi figuru 1-4: ";
		int c;
		cin >> c;
		cin.ignore();

		if (c >= 1 && c <= 4) {
			int index = c - 1;
			if (!pieces[index].is_in_base() && !pieces[index].is_in_goal() && pieces[index].can_move(dice)) {
				return index;
			}
			cout << "ta figura nije dostupna" << endl;
		}
		else { cout << "neispravan unos" << endl; }
	}
}

bool HumanPlayer::new_or_move() {
	cout << "dobio/la si 6. stisni 1 za novu figuru ili 2 za pomicanje postojece: ";

	int c;
	cin >> c;
	cin.ignore();
	return (c == 1);
}