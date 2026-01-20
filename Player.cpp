#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "Graphics.h"

using namespace std;

Player::Player(const string& player_name, const string& player_color, int start_pos)
	:name(player_name), color(player_color), start_position(start_pos)
{
	for (int i = 0; i < NUM_PIECES_PER_PLAYER; i++) {
		pieces[i].init(start_position);
	}
	for (int i = 0; i < HOME_STEPS; i++) {
		home_occupied[i] = false;
	}
}

Player::~Player() {}

void Player::set_game(Game* g) { game_ref = g; }

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

void Player::set_player_index(int index) { player_index = index; }

int Player::get_player_index() const { return player_index; }

bool Player::has_piece_on_board() const {
	for (const auto& piece : pieces) {
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
		if (piece.is_in_goal() || piece.is_in_home()) { cnt++; }
	}
	return cnt;
}

int Player::find_available_home(int needed_steps) {
	int home_index = needed_steps - BOARD_SIZE;

	if (home_index >= 0 && home_index < HOME_STEPS) {
		if (!home_occupied[home_index]) {
			return needed_steps;
		}
	}
	return -1;
}


void Player::handle_rolling_to_game(int& dice) {
	for (int i = 0; i < MAX_ATTEMPTS_TO_ROLL; i++) {
		dice = roll_dice();
		//cout << "dobio/la si: " << dice << endl;

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

	if (new_steps >= BOARD_SIZE) {

		if (curr_steps >= BOARD_SIZE) {
			int old_home_index = curr_steps - BOARD_SIZE;
			if (old_home_index >= 0 && old_home_index < HOME_STEPS) {
				home_occupied[old_home_index] = false;
			}
		}

		if (new_steps == TOTAL_STEPS_TO_GOAL) {
			home_occupied[3] = true;
			piece.move_to_goal();
		}
		else if (new_steps > TOTAL_STEPS_TO_GOAL) {
			int overfl = new_steps - BOARD_SIZE;
			int new_pos = (start_position + overfl) % BOARD_SIZE;

			for (int k = 0; k < NUM_PIECES_PER_PLAYER; k++) {
				if (k != index && !pieces[k].is_in_base() && !pieces[k].is_in_goal() &&
					!pieces[k].is_in_home() && pieces[k].get_position() == new_pos) {
					return false;
				}
			}
			last_move_pos = new_pos;
			piece.move_to_position(overfl, last_move_pos);
			board->place(&piece, last_move_pos);
		}
		else {
			int avail_steps = find_available_home(new_steps);

			if (avail_steps == -1) {
				int overfl = new_steps - BOARD_SIZE;
				int new_pos = (start_position + overfl) % BOARD_SIZE;

				for (int k = 0; k < NUM_PIECES_PER_PLAYER; k++) {
					if (k != index && !pieces[k].is_in_base() && !pieces[k].is_in_goal() &&
						!pieces[k].is_in_home() && pieces[k].get_position() == new_pos) {
						return false;
					}
				}
				last_move_pos = new_pos;
				piece.move_to_position(overfl, last_move_pos);
				board->place(&piece, last_move_pos);
			}
			else {
				int home_index = avail_steps - BOARD_SIZE;
				home_occupied[home_index] = true;

				int home_pos = HOME_POSITION_BASE + start_position + home_index;
				piece.move_to_position(avail_steps, home_pos);
			}
		}
	}
	else {

		int new_pos = (start_position + curr_steps + dice) % BOARD_SIZE;

		bool is_my_piece = false;

		for (int k = 0; k < NUM_PIECES_PER_PLAYER; k++) {
			if (k != index && !pieces[k].is_in_base() && !pieces[k].is_in_goal() &&
				!pieces[k].is_in_home() && pieces[k].get_position() == new_pos) {
				is_my_piece = true;
				break;
			}
		}

		if (is_my_piece) {
			//cout << "Ne mozes stat na svoju figuru!" << endl;
			return false;
		}


		piece.move(dice);
		last_move_pos = piece.get_position();
		board->place(&piece, last_move_pos);
	}

	if (last_move_pos >= 0 && last_move_pos < BOARD_SIZE) {
		string eaten = board->eat_enemy(&piece, last_move_pos);
		if (!eaten.empty() && game_ref && game_ref->get_graphics()) {
			string msg = color + " je pojeo " + eaten + "!";
			game_ref->get_graphics()->set_message(msg);
		}
	}
	return true;
}

void Player::take_piece_from_base(Board* board) {
	for (int i = 0; i < NUM_PIECES_PER_PLAYER; i++) {
		if (!pieces[i].is_in_base() && !pieces[i].is_in_goal() &&
			!pieces[i].is_in_home() && pieces[i].get_position() == start_position) {
			return; 
		}
	}
	for (int i = 0; i < NUM_PIECES_PER_PLAYER; i++) {
		if (pieces[i].is_in_base()) {
			pieces[i].place_on_start();
			last_move_pos = pieces[i].get_position();
			board->place(&pieces[i], last_move_pos);

			string eaten = board->eat_enemy(&pieces[i], last_move_pos);
			if (!eaten.empty() && game_ref && game_ref->get_graphics()) {
				string msg = color + " je pojeo " + eaten + "!";
				game_ref->get_graphics()->set_message(msg);
			}			break;
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
		if (new_or_move()) {
			take_piece_from_base(board);
		}
		else { execute_move(dice, board); };
	}
	else { execute_move(dice, board); };


	int second_dice = roll_dice();
	if (second_dice == MAX_DICE_VALUE) {
		handle_six(second_dice, board);
	}
	else if (has_valid_move(second_dice)) { execute_move(second_dice, board); }
}

void Player::handle_normal_move(int dice, Board* board) { execute_move(dice, board); };


void Player::play_turn(Board* board) {
	//cout << endl;
	//cout << "sad igra: " << name << " (" << color << ")" << endl;
	//cout << "u bazi: " << cnt_pieces_in_base() << "/4" << endl;
	//cout << "u kucici: " << cnt_pieces_in_goal() << "/4" << endl;

	int dice = 0;

	try {
		if (!has_piece_on_board() && cnt_pieces_in_base() > 0) {
			handle_rolling_to_game(dice);
			if (dice != MAX_DICE_VALUE) { return; }
			take_piece_from_base(board);

			//cout << "baci opet!" << endl;
			int second = roll_dice();
			//cout << "kockica: " << second << endl;
			if (has_valid_move(second)) {
				handle_six(second, board);
			}
		}
		else if (cnt_pieces_in_base() == 0 && !has_piece_on_board()) {
			dice = roll_dice();
			//cout << "kockica: " << dice << endl;
			if (has_valid_move(dice)) {
				execute_move(dice, board);
				if (dice == MAX_DICE_VALUE) {
					//cout << "baci opet!" << endl;
					int second = roll_dice();
					//cout << "kockica: " << second << endl;
					if (has_valid_move(second)) {
						execute_move(second, board);
					}
				}
			}
		}
		else {
			dice = roll_dice();
			//cout << "kockica: " << dice << endl;
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
		//cout << name << " je pobjednik!!!" << endl;
	}

}
