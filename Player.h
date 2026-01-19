#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Piece.h"
#include "GameConstants.h"

using namespace std;

class Player {
protected:
	string name;
	string color;
	Piece pieces[GameConstants::NUM_PIECES_PER_PLAYER];
	int start_position;

	virtual int roll_dice() = 0;
	virtual int choose_piece(int dice) = 0;
	virtual bool new_or_move() = 0;
	//virtual void handle_six_with_pieces_out(int dice);

	bool execute_move(int dice);

	void handle_rolling_to_game(int& dice);
	void handle_six(int dice);
	void handle_normal_move(int dice);
	void take_piece_from_base();
public:
	Player(const string& player_name, const string& player_color, int start_pos);

	virtual ~Player();

	string get_name() const;
	string get_color() const;
	int get_start_pos() const;
	Piece* get_piece(int index);

	bool has_piece_on_board() const;
	bool all_pieces_in_goal() const;
	int cnt_pieces_in_base() const;
	int cnt_pieces_in_goal() const;
	bool has_valid_move(int dice) const;

	virtual void play_turn();
};

#endif
