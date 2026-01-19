#include <iostream>
#include <cstdlib>
#include "ComputerPlayer.h"

using namespace std;

ComputerPlayer::ComputerPlayer(string player_name, string player_color, int start_pos) : Player(player_name, player_color, start_pos) {}

int ComputerPlayer::roll_dice() {
	int result = (rand() % MAX_DICE_VALUE) + 1;
	return result;
}

int ComputerPlayer::choose_piece(int dice) {
	cout << "kompjuter bira figuru..." << endl;

	for (int i = 0; i < NUM_PIECES_PER_PLAYER; i++) {
		if (!pieces[i].is_in_base() && !pieces[i].is_in_goal() && pieces[i].can_move(dice)) {
			cout << "racunalo je odabralo figuru " << (i + 1) << endl;
			return i;
		}
	}
	return -1;
}

bool ComputerPlayer::new_or_move() {

	if (cnt_pieces_in_base() >= 2) {
		cout << "racunalo izvlaci novu figuru" << endl;
		return true;
	}
	cout << "racunalo pomice postojecu figuru" << endl;
	return false;
}
