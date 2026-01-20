#include "HumanPlayer.h"
#include "Game.h"
#include "Graphics.h"
#include <iostream>
#include <cstdlib>

using namespace std;

HumanPlayer::HumanPlayer(const string& name, const string& color, int start_pos) : Player(name, color, start_pos) {};

void HumanPlayer::set_graphics(Graphics* graph) { graphics = graph; }

int HumanPlayer::roll_dice() {
	//cout << "stisni enter za bacit kockicu" << endl;
	//cin.get();
	int result;

	if (graphics) { 
		result = graphics->wait_for_roll(); 
	}
	else {
		result = (rand() % MAX_DICE_VALUE) + 1;
	}
	
	if (game_ref) { game_ref->update_dice(result); }
	
	return result;
}

int HumanPlayer::choose_piece(int dice) {
	/*for (int i = 0; i < NUM_PIECES_PER_PLAYER; i++) {
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
		}
	}*/
	if (graphics && game_ref) {
		if (graphics && player_index >= 0) {
			return graphics->wait_for_piece_selection(player_index, dice);
		}
	}
	return 0;
}


bool HumanPlayer::new_or_move() {
	if (graphics) {
		return graphics->wait_for_new_or_move();
	}
	return true;
}