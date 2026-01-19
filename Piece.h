#ifndef PIECE_H
#define PIECE_H

#include <iostream>
using namespace std;
class Piece {
private:
	int position;
	int steps_taken;
	int owner_start_pos;
	bool in_base;
	bool in_goal;
public:
	Piece();

	void init(int start_pos);

	int get_position() const;
	int get_steps_taken() const;
	int get_owner_start() const;

	bool is_in_base() const;
	bool is_in_goal() const;
	
	void place_on_start();
	bool can_move(int dice) const;
	int new_position(int dice) const;

	void move(int dice);
	void move_to_position(int steps, int pos);
	void move_to_goal();
	void return_to_base();
};

#endif
