#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include "Player.h"

using namespace std;

Player::Player(const string& player_name,const string& player_color, int start_pos) 
	:name(player_name), color(player_color), start_position(start_pos) 
{
	for (int i = 0; i < GameConstants::NUM_PIECES_PER_PLAYER; i++) {
		pieces[i].init(start_position);
	}
	for (int i = 0; i < GameConstants::HOME_STEPS; i++) {
		home_occupied[i] = false;
	}
}

Player::~Player(){}

string Player::get_name() const { return name; };
string Player::get_color() const { return color; };
int Player::get_start_pos() const { return start_position; };

Piece* Player::get_piece(int i) {
	if (i >= 0 && i < GameConstants::NUM_PIECES_PER_PLAYER) {
		return &pieces[i];
	}
	return nullptr;
}

bool Player::has_piece_on_board() const {
	for (const auto& piece: pieces) {
		if (!piece.is_in_base() && !piece.is_in_goal()) {
			return true;
		} 
	}
	return false;
}

bool Player::all_pieces_in_goal() const {
	for (const auto& piece : pieces) {
		if (!piece.is_in_goal()) {
			return false;
		}
	}
	return true;
}

int Player::cnt_pieces_in_base() const {
	auto in_base = [](const Piece& p) { return p.is_in_base(); };
	return count_if(begin(pieces), end(pieces), in_base);
}

int Player::cnt_pieces_in_goal() const {
	auto in_goal = [](const Piece& p) { return p.is_in_goal(); };
	return count_if(begin(pieces), end(pieces), in_goal);
}

int Player::find_available_home(int needed_steps) {
	for (int s = needed_steps; s > GameConstants::BOARD_SIZE; s--) {
		int home_index = s - GameConstants::BOARD_SIZE - 1;

		if (home_index >= 0 && home_index < GameConstants::HOME_STEPS) {
			if (!home_occupied[home_index]) { return s; }
		}
	}
	return -1;
}

void Player::handle_rolling_to_game(int& dice) {
	for (int i = 0; i < GameConstants::MAX_ATTEMPTS_TO_ROLL;i++) {
		dice = roll_dice();
		cout << "dobio/la si: " << dice << endl;

		if (dice == GameConstants::MAX_DICE_VALUE) { break; };
	}
}

bool Player::has_valid_move(int dice) const {
	if (dice == GameConstants::MAX_DICE_VALUE && cnt_pieces_in_base() > 0) {
		return true;
	}
	for (const auto& p : pieces) {
		if (!p.is_in_base() && !p.is_in_goal() && p.can_move(dice)) { 
			return true; 
		};
	}
	return false;
}

bool Player::execute_move(int dice) {
	if (!has_valid_move(dice)) { return false; };

	int index = choose_piece(dice);
	
	if (index == -1) { return false; };

	Piece& piece = pieces[index];
	int curr_steps = piece.get_steps_taken();
	int new_steps = curr_steps + dice;

	if (new_steps > GameConstants::BOARD_SIZE) {

		if (curr_steps > GameConstants::BOARD_SIZE) {
			int old_home_index = curr_steps - GameConstants::BOARD_SIZE - 1;
			if (old_home_index >= 0 && old_home_index < GameConstants::HOME_STEPS) {
				home_occupied[old_home_index] = false;
			}
		}

		int avail_steps = find_available_home(new_steps);

		if (avail_steps == -1) {
			int overfl = new_steps % GameConstants::BOARD_SIZE;
			piece.move_to_position(overfl, (start_position + overfl) % GameConstants::BOARD_SIZE);
			cout << "nema mista u kucici, ides u novi krug na " << piece.get_position() << endl;
		}
		else {
			int home_index = avail_steps - GameConstants::BOARD_SIZE - 1;
			home_occupied[home_index] = true;

			if (avail_steps == GameConstants::TOTAL_STEPS_TO_GOAL) {
				piece.move_to_goal();
				cout << "figura u kucici" << endl;
			}
			else {
				int home_pos = GameConstants::HOME_POSITION_BASE + start_position + home_index;
				piece.move_to_position(avail_steps, home_pos);
				cout << "figura u kucici na mistu " << (home_pos + 1) << "/4" << endl;
			}
		}
	}
	else { piece.move(dice); }

	return true;
}

void Player::take_piece_from_base() {
	for (int i = 0; i < GameConstants::NUM_PIECES_PER_PLAYER; i++) {
		if (pieces[i].is_in_base()) {
			pieces[i].place_on_start();
			break;
		}
	}
}

void Player::handle_six(int dice) {
	bool has_in_base = (cnt_pieces_in_base() > 0);
	bool has_on_board = has_piece_on_board();

	if (has_in_base && !has_on_board) {
		for (int i = 0; i < GameConstants::NUM_PIECES_PER_PLAYER; i++) {
			if (pieces[i].is_in_base()) {
				pieces[i].place_on_start();
				break;
			}
		}
	}
	else if (has_in_base && has_on_board) {
		//nova
		if (new_or_move()) {
			for (int i = 0; i < GameConstants::NUM_PIECES_PER_PLAYER; i++) {
				if (pieces[i].is_in_base()) {
					pieces[i].place_on_start();
					break;
				}
			}
		} // postojeca
		else { execute_move(dice); };
	}
	else { execute_move(dice); };


	cout << "baci opet" << endl;
	int second_dice = roll_dice();
	cout << "kockica " << second_dice << endl;

	if (has_valid_move(second_dice)) { execute_move(second_dice); }
}

void Player::handle_normal_move(int dice) { execute_move(dice); };

void Player::play_turn() {
	cout << "sad igra: " << name << " (" << color << ")" << endl;
	cout << "u bazi: " << cnt_pieces_in_base() << "/4" << endl;
	cout << "u kucici: " << cnt_pieces_in_goal() << "/4" << endl;

	int dice = 0;

	try {
		if (!has_piece_on_board()) {
			handle_rolling_to_game(dice);
			if (dice != GameConstants::MAX_DICE_VALUE) { return; }
		}
		else {
			dice = roll_dice();
			cout << "kockica: " << dice << endl;
		}

		if (dice == GameConstants::MAX_DICE_VALUE) { 
			handle_six(dice); 
		}
		else { handle_normal_move(dice); }
	}
	catch (const exception& e) {
		cout << "greska: " << e.what() << endl;
	
	}

	if (all_pieces_in_goal()) { 
		cout << name << " je pobjednik!!!" << endl; 
	}

}
