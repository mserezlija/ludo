#include <iostream>
#include <cstdlib>
#include "ComputerPlayer.h"
#include "Graphics.h"
#include "Game.h"

using namespace std;

ComputerPlayer::ComputerPlayer(string player_name, string player_color, int start_pos) : Player(player_name, player_color, start_pos) {}

int ComputerPlayer::roll_dice() {

	if (game_ref && game_ref->get_graphics()) {
		for (int i = 0; i < 48; i++) {
			game_ref->get_graphics()->update();
		}
	}

	int result = (rand() % MAX_DICE_VALUE) + 1;
	if (game_ref) {
		game_ref->update_dice(result);
	}
	return result;
}

int ComputerPlayer::choose_piece(int dice) {

	for (int i = 0; i < NUM_PIECES_PER_PLAYER; i++) {
		if (!pieces[i].is_in_base() && !pieces[i].is_in_goal() && pieces[i].can_move(dice)) {
			return i;
		}
	}
	return -1;
}

bool ComputerPlayer::new_or_move() {

	if (cnt_pieces_in_base() >= 1) { return true; }
	return false;
}
