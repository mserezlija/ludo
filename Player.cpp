#include <iostream>
#include <cstdlib>
#include "Player.h"

using namespace std;

Player::Player(string player_name, string player_color, int start_pos) :name(player_name), color(player_color), start_position(start_pos) {}

Player::~Player() {}

string Player::get_name() const { return name; };

string Player::get_color() const { return color; };

int Player::roll_dice() {
	cout << "stisni ENTER da bacis kockicu" << endl;
	cin.get();
	int result = (rand() % 6) + 1;
	return result;
}

bool Player::has_piece_out_of_base() const {
	for (const auto& piece : pieces) {
		if (!piece.is_in_base()) {
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
	int cnt = 0;
	for (const auto& piece : pieces) {
		if (piece.is_in_base())
			cnt++;
	}
	return cnt;
}

void Player::show_available_pieces() const {
	cout << "dostupne figure: " << endl;
	for (int i = 0; i < 4; i++) {
		if (!pieces[i].is_in_base() && !pieces[i].is_in_goal()) {
			cout << "figura " << (i + 1) << " - pozicija: " << pieces[i].get_position() << endl;
		}
	}
}

int Player::choose_piece() {
	show_available_pieces();
	cout << "odaberi figuru 1-4" << endl;
	int piece_num;
	cin >> piece_num;
	cin.ignore();

	if (piece_num >= 1 && piece_num <= 4) {
		return piece_num - 1;
	}
	return -1;
}

void Player::handle_rolling_to_game(int& dice) {
	int attempts = 0;
	while (attempts < 3 && !has_piece_out_of_base()) {
		attempts++;
		dice = roll_dice();
		cout << "dobio/la si: " << dice << endl;

		if (dice == 6) {
			break;
		}
	}
}

void Player::handle_six_no_pieces_out(int dice) {
	for (int i = 0; i < 4; i++) {
		if (pieces[i].is_in_base()) {
			pieces[i].place_on_start(start_position);
			cout << "izvukao/la si figuru na plocu! bacaj oept" << endl;

			int second_dice = roll_dice();
			cout << "dobio/la si: " << second_dice << endl;
			pieces[i].move(second_dice);
			break;
		}
	}
}

void Player::handle_six_with_pieces_out(int dice) {
	cout << "dobio/la si 6! odaberi 1 za novu figuru ili 2 za pomaknuti postojecu " << endl;
	int choice;
	cin >> choice;
	cin.ignore();

	if (choice == 1 && cnt_pieces_in_base() > 0) {
		for (int i = 0; i < 4; i++) {
			if (pieces[i].is_in_base()) {
				pieces[i].place_on_start(start_position);
				break;
			}
		}
	}
	else {
		int index = choose_piece();
		if (index != -1 && !pieces[index].is_in_base() && !pieces[index].is_in_goal()) {
			pieces[index].move(6);
		}
	}

	cout << "baci opet!" << endl;
	int second_dice = roll_dice();
	cout << "dobio/la si: " << second_dice << endl;

	int index = choose_piece();
	if (index != -1 && !pieces[index].is_in_base() && !pieces[index].is_in_goal()) {
		pieces[index].move(second_dice);
	}
}

void Player::handle_normal_move(int dice) {
	int index = choose_piece();
	if (index != -1 && !pieces[index].is_in_base() && !pieces[index].is_in_goal()) {
		pieces[index].move(dice);
	}
}

void Player::play_turn() {
	cout << "sad igra: " << name << " (" << color << ")" << endl;

	int dice = 0;

	if (!has_piece_out_of_base()) {
		handle_rolling_to_game(dice);
	}
	else {
		dice = roll_dice();
		cout << "dobio/la si: " << dice << endl;
	}

	if (dice == 6 && !has_piece_out_of_base()) {
		handle_six_no_pieces_out(dice);
	}
	else if (dice == 6 && has_piece_out_of_base()) {
		handle_six_with_pieces_out(dice);
	}
	else if (has_piece_out_of_base()) {
		handle_normal_move(dice);
	}
	else {
		cout << "nemas figuru za igranje" << endl;
	}

	cout << endl;
	cout << "u bazi: " << cnt_pieces_in_base() << "/4" << endl;
	cout << "u kucici: " << (all_pieces_in_goal() ? "sve!" : "joj, ne sve") << endl;
}
