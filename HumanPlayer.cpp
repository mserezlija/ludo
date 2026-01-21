#include "HumanPlayer.h"
#include "Game.h"
#include "Graphics.h"
#include <iostream>
#include <cstdlib>

using namespace std;

HumanPlayer::HumanPlayer(const string& name, const string& color, int start_pos) : Player(name, color, start_pos) {};

void HumanPlayer::set_graphics(Graphics* graph) { graphics = graph; }

int HumanPlayer::roll_dice() {
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