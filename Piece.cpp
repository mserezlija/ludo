#include "Piece.h"
#include "GameConstants.h"
#include <stdexcept>

Piece::Piece() 
	:position(GameConstants::BASE_POSITION),steps_taken(0), owner_start_pos(0), in_base(true), in_goal(false) {}


void Piece::init(int start_pos) { owner_start_pos = start_pos; };

int Piece::get_position() const { return position; };
int Piece::get_steps_taken() const { return steps_taken; };
int Piece::get_owner_start() const { return owner_start_pos; };

bool Piece::is_in_base() const { return in_base; };
bool Piece::is_in_goal() const { return in_goal; };

void Piece::place_on_start() {
	if (!in_base) { throw runtime_error("Figura nije u bazi, ne moze izaci na plocu!"); };
	
	position = owner_start_pos;
	steps_taken = 0;
	in_base = false;
	in_goal = false;
	
	cout << "figura izasla iz baze na poziciju: " << position << endl;
}

bool Piece::can_move(int dice) const { return !in_base && !in_goal; };

int Piece::new_position(int dice) const {
	if (in_base || in_goal) { return GameConstants::BASE_POSITION; };

	int new_steps = steps_taken + dice;
	int go_home = GameConstants::HOME_POSITION_BASE + owner_start_pos + (new_steps - GameConstants::BOARD_SIZE - 1);
	int continue_to_board = (owner_start_pos + new_steps) % GameConstants::BOARD_SIZE;

	if (new_steps > GameConstants::BOARD_SIZE) {
		return go_home;
	}
	return continue_to_board;
}

void Piece::move(int dice) {
	if (in_base) { throw runtime_error("Figura je u bazi, koristi drugu funkciju!"); };
	if (in_goal) { throw runtime_error("Figura je u kucici, koristi drugu funkciju!"); };
	
	int new_steps = steps_taken + dice;

	if (steps_taken > GameConstants::BOARD_SIZE && new_steps > GameConstants::TOTAL_STEPS_TO_GOAL) {
		throw runtime_error("greska!!!");
	}
	if (steps_taken == GameConstants::TOTAL_STEPS_TO_GOAL) {
		in_goal = true;
		position = GameConstants::BASE_POSITION;
		cout << "figura u kucici" << endl;
		return;
	} else if (steps_taken > GameConstants::BOARD_SIZE && new_steps<GameConstants::TOTAL_STEPS_TO_GOAL) {
		position = GameConstants::HOME_POSITION_BASE + owner_start_pos + (new_steps - GameConstants::BOARD_SIZE - 1);
		cout << "figura ulazi u kucicu: " << (new_steps - GameConstants::BOARD_SIZE) << "/4" << endl;
	}
	else {
		position = (owner_start_pos + new_steps) % GameConstants::BOARD_SIZE;
		cout << "figura se pomakla na poziciju: " << position << endl;
	}
	steps_taken = new_steps;
}

void Piece::return_to_base() {
	position = GameConstants::BASE_POSITION;
	steps_taken = 0;
	in_base = true;
	in_goal = false;
	cout << "figura se vraca u bazu." << endl;
}