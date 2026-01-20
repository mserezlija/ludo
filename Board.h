#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "GameConstants.h"

class Game;

class Board {
private:
	Piece* board_fields[BOARD_SIZE];
	Game* game;
public:
	Board(Game* g);

	bool is_avail(int pos) const;
	void place(Piece* piece, int pos);
	void clear_pos(int pos);
	string eat_enemy(Piece* piece, int pos);
	Piece* get_piece_at(int pos) const;
};

#endif 
