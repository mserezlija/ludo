#include <iostream>
#include "ComputerPlayer.h"

using namespace std;

ComputerPlayer::ComputerPlayer(string player_name, string player_color, int start_pos) : Player(player_name, player_color, start_pos) {}

int ComputerPlayer::roll_dice() {
	int result = (rand() % 6) + 1;
	return result;
}

int ComputerPlayer::choose_piece() {
	cout << "kompjuter bira figuru..." << endl;

	for (int i = 0; i < 4; i++) {
		if (!pieces[i].is_in_base() && !pieces[i].is_in_goal()) {
			cout << "racunalo je odabralo figuru " << (i + 1) << endl;
			return i;
		}
	}
	return -1;
}

void ComputerPlayer::handle_six_with_pieces_out(int dice) {
	cout << "komp je dobio 6" << endl;

	if (cnt_pieces_in_base() > 0) {
		cout << "komp izvlaci novu figuru" << endl;
		for (int i = 0; i < 4; i++) {
			if (pieces[i].is_in_base()) {
				pieces[i].place_on_start(start_position);
				break;
			}
		}
	}
	else {
		cout << "komp pomice figuru" << endl;
		int index = choose_piece();
		if (index != -1 && !pieces[index].is_in_base() && !pieces[index].is_in_goal()) {
			pieces[index].move(6);
		}
	}

	//drugo bacanje
	cout << "komp opet baca kockicu" << endl;
	int second_dice = roll_dice();
	cout << "dobio/la si: " << second_dice << endl;

	int index = choose_piece();
	if (index != -1 && !pieces[index].is_in_base() && !pieces[index].is_in_goal()) {
		pieces[index].move(second_dice);
	}

}

void ComputerPlayer::play_turn() {
	Player::play_turn();
}






