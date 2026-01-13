#ifndef PIECE_H
#define PIECE_H

#include <iostream>
using namespace std;
class Piece {
private:
	int position;
	bool in_base;
	bool in_goal;
public:
	Piece();

	int get_position() const;
	bool is_in_base() const;
	bool is_in_goal() const;

	void place_on_start(int start_pos);
	void move(int num_fields);
	void return_to_base();
};

#endif
