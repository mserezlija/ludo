#include "Piece.h"
#include "GameConstants.h"
#include <stdexcept>

using namespace std;

Piece::Piece()
	:position(BASE_POSITION), steps_taken(0), owner_start_pos(0), in_base(true), in_goal(false) {
}


void Piece::init(int start_pos) { owner_start_pos = start_pos; };

int Piece::get_position() const { return position; };
int Piece::get_steps_taken() const { return steps_taken; };
int Piece::get_owner_start() const { return owner_start_pos; };

bool Piece::is_in_base() const { return in_base; };
bool Piece::is_in_goal() const { return in_goal; };
bool Piece::is_in_home() const { return steps_taken > BOARD_SIZE && !in_goal; }

void Piece::place_on_start() {
	if (!in_base) { throw runtime_error("Figura nije u bazi, ne moze izaci na plocu!"); };

	position = owner_start_pos;
	steps_taken = 0;
	in_base = false;
	in_goal = false;

	cout << "figura izasla iz baze na poziciju: " << position << endl;
}

bool Piece::can_move(int dice) const {
	if (in_base || in_goal) return false;

	if (steps_taken > BOARD_SIZE) {
		int new_steps = steps_taken + dice;
		return new_steps <= TOTAL_STEPS_TO_GOAL;
	}
	return true;
}

int Piece::new_position(int dice) const {
	if (in_base || in_goal) { return BASE_POSITION; };

	int new_steps = steps_taken + dice;
	int go_home = HOME_POSITION_BASE + owner_start_pos + (new_steps - BOARD_SIZE - 1);
	int continue_to_board = (owner_start_pos + new_steps) % BOARD_SIZE;

	if (new_steps > BOARD_SIZE) {
		return go_home;
	}
	return continue_to_board;
}

void Piece::move(int dice) {
	if (in_base) { throw runtime_error("Figura je u bazi, koristi drugu funkciju!"); };
	if (in_goal) { throw runtime_error("Figura je u kucici, koristi drugu funkciju!"); };

	steps_taken += dice;
	position = (owner_start_pos + steps_taken) % BOARD_SIZE;

	cout << "figura se pomakla na poz: " << position << endl;
}

void Piece::move_to_position(int steps, int pos) {
	if (in_base) { throw runtime_error("Figura je u bazi, koristi drugu funkciju!"); };
	if (in_goal) { throw runtime_error("Figura je u kucici, koristi drugu funkciju!"); };

	steps_taken = steps;
	position = pos;
}

void Piece::move_to_goal() {
	if (in_base) { throw runtime_error("Figura je u bazi, koristi drugu funkciju!"); };

	in_goal = true;
	position = BASE_POSITION;
	steps_taken = TOTAL_STEPS_TO_GOAL;
}

void Piece::return_to_base() {
	position = BASE_POSITION;
	steps_taken = 0;
	in_base = true;
	in_goal = false;
	cout << "figura se vraca u bazu." << endl;
}