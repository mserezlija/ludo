#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include "Player.h"
#include "Board.h"

using namespace std;

Player::Player(const string& player_name,const string& player_color, int start_pos) 
	:name(player_name), color(player_color), start_position(start_pos) 
{
	for (int i = 0; i < NUM_PIECES_PER_PLAYER; i++) {
		pieces[i].init(start_position);
	}
	for (int i = 0; i < HOME_STEPS; i++) {
		home_occupied[i] = false;
	}
}

Player::~Player(){}

string Player::get_name() const { return name; };
string Player::get_color() const { return color; };
int Player::get_start_pos() const { return start_position; };
int Player::get_last_move_position() const { { return last_move_pos; } }

Piece* Player::get_piece(int i) {
	if (i >= 0 && i < NUM_PIECES_PER_PLAYER) {
		return &pieces[i];
	}
	return nullptr;
}

bool Player::has_piece_on_board() const {
	for (const auto& piece: pieces) {
		if (!piece.is_in_base() && !piece.is_in_goal() && !piece.is_in_home()) {
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
	int cnt = 0;
	for (const auto& piece : pieces) {
		if (piece.is_in_goal() ||  piece.is_in_home()) { cnt++; }
	}
	return cnt;
}

int Player::find_available_home(int needed_steps) {
	for (int s = needed_steps; s > BOARD_SIZE; s--) {
		int home_index = s - BOARD_SIZE - 1;

		if (home_index >= 0 && home_index < HOME_STEPS) {
			if (!home_occupied[home_index]) { return s; }
		}
	}
	return -1;
}

void Player::handle_rolling_to_game(int& dice) {
	for (int i = 0; i < MAX_ATTEMPTS_TO_ROLL;i++) {
		dice = roll_dice();
		cout << "dobio/la si: " << dice << endl;

		if (dice == MAX_DICE_VALUE) { break; };
	}
}

bool Player::has_valid_move(int dice) const {
	if (dice == MAX_DICE_VALUE && cnt_pieces_in_base() > 0) {
		return true;
	}
	for (const auto& p : pieces) {
		if (!p.is_in_base() && !p.is_in_goal() && p.can_move(dice)) { 
			return true; 
		};
	}
	return false;
}

bool Player::execute_move(int dice, Board* board) {
	if (!has_valid_move(dice)) { return false; }

	int index = choose_piece(dice);

	if (index == -1) { return false; }

	Piece& piece = pieces[index];
	int old_position = piece.get_position();
	int curr_steps = piece.get_steps_taken();
	int new_steps = curr_steps + dice;

	if (old_position >= 0 && old_position < BOARD_SIZE && !piece.is_in_home()) {
		board->clear_pos(old_position);
	}

	last_move_pos = -1;

	if (new_steps > BOARD_SIZE) {

		if (curr_steps > BOARD_SIZE) {
			int old_home_index = curr_steps - BOARD_SIZE - 1;
			if (old_home_index >= 0 && old_home_index < HOME_STEPS) {
				home_occupied[old_home_index] = false;
			}
		}

		if (new_steps == TOTAL_STEPS_TO_GOAL) {
			home_occupied[3] = true;
			piece.move_to_goal();
			cout << "FIGURA U CILJU!" << endl;
		}
		else if (new_steps > TOTAL_STEPS_TO_GOAL) {
			int overfl = new_steps - BOARD_SIZE;
			last_move_pos = (start_position + overfl) % BOARD_SIZE;
			piece.move_to_position(overfl, last_move_pos);
			board->place(&piece, last_move_pos);
			cout << "ne moze u kucicu, nastavlja na " << last_move_pos << endl;
		}
		else {
			int avail_steps = find_available_home(new_steps);

			if (avail_steps == -1) {
				int overfl = new_steps - BOARD_SIZE;
				last_move_pos = (start_position + overfl) % BOARD_SIZE;
				piece.move_to_position(overfl, last_move_pos);
				board->place(&piece, last_move_pos);
				cout << "nema mista u kucici, ides na " << last_move_pos << endl;
			}
			else {
				int home_index = avail_steps - BOARD_SIZE - 1;
				home_occupied[home_index] = true;

				int home_pos = HOME_POSITION_BASE + start_position + home_index;
				piece.move_to_position(avail_steps, home_pos);
				cout << "figura u kucici na mistu " << (home_index + 1) << "/4" << endl;
			}
		}
	}
	else {
		piece.move(dice);
		last_move_pos = piece.get_position();
		board->place(&piece, last_move_pos);
	}

	if (last_move_pos >= 0 && last_move_pos < BOARD_SIZE) {
		board->eat_enemy(&piece, last_move_pos);
	}
	return true;
}

void Player::take_piece_from_base(Board* board) {
	for (int i = 0; i < NUM_PIECES_PER_PLAYER; i++) {
		if (pieces[i].is_in_base()) {
			pieces[i].place_on_start();
			last_move_pos = pieces[i].get_position();
			board->place(&pieces[i], last_move_pos);

			board->eat_enemy(&pieces[i], last_move_pos);
			break;
		}
	}
}

void Player::handle_six(int dice, Board* board) {
	bool has_in_base = (cnt_pieces_in_base() > 0);
	bool has_on_board = has_piece_on_board();

	if (has_in_base && !has_on_board) {
		take_piece_from_base(board);
	}
	else if (has_in_base && has_on_board) {
		//nova
		if (new_or_move()) {
			take_piece_from_base(board);
		} // postojeca
		else { execute_move(dice, board); };
	}
	else { execute_move(dice, board); };


	cout << "baci opet" << endl;
	int second_dice = roll_dice();
	cout << "kockica " << second_dice << endl;

	if (has_valid_move(second_dice)) { execute_move(second_dice, board); }
}

void Player::handle_normal_move(int dice, Board* board) { execute_move(dice, board); };

void Player::play_turn(Board* board) {
	cout << endl;
	cout << "sad igra: " << name << " (" << color << ")" << endl;
	cout << "u bazi: " << cnt_pieces_in_base() << "/4" << endl;
	cout << "u kucici: " << cnt_pieces_in_goal() << "/4" << endl;

	int dice = 0;

	try {
		if (!has_piece_on_board() && cnt_pieces_in_base() > 0) {
			handle_rolling_to_game(dice);
			if (dice != MAX_DICE_VALUE) { return; }
			take_piece_from_base(board);

			cout << "baci opet!" << endl;
			int second = roll_dice();
			cout << "kockica: " << second << endl;
			if (has_valid_move(second)) {
				execute_move(second, board);
			}
		}
		else if (cnt_pieces_in_base() == 0 && !has_piece_on_board()) {
			dice = roll_dice();
			cout << "kockica: " << dice << endl;
			if (has_valid_move(dice)) {
				execute_move(dice, board);
				if (dice == MAX_DICE_VALUE) {
					cout << "baci opet!" << endl;
					int second = roll_dice();
					cout << "kockica: " << second << endl;
					if (has_valid_move(second)) {
						execute_move(second, board);
					}
				}
			}
		}
		else {
			dice = roll_dice();
			cout << "kockica: " << dice << endl;
			if (dice == MAX_DICE_VALUE) {
				handle_six(dice, board);
			}
			else {
				handle_normal_move(dice, board);
			}
		}
	}
	catch (const exception& e) {
		cout << "greska: " << e.what() << endl;
	
	}

	if (all_pieces_in_goal()) { 
		cout << name << " je pobjednik!!!" << endl; 
	}

}
