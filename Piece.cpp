#include "Piece.h"

Piece::Piece() : position(-1), in_base(true), in_goal(false) {}

int Piece::get_position() const {
	return position;
}

bool Piece::is_in_base() const {
	return in_base;
}

bool Piece::is_in_goal() const {
	return in_goal;
}

void Piece::place_on_start(int start_pos) {
	position = start_pos;
	in_base = false;
	cout << "figura izasla iz baze na poziciju: " << position << endl;
}

void Piece::move(int num_fields) {
	if (!in_base && !in_goal) {
		position += num_fields;

		if (position >= 40) {
			in_goal = true;
			cout << "figura u kucici!" << endl;
		}
		else {
			cout << "figura se pomakla na poziciju: " << position << endl;
		}
	}
}

void Piece::return_to_base() {
	position = -1;
	in_base = true;
	in_goal = false;
	cout << "pokosilo nas je! figura se vraca u bazu." << endl;
}