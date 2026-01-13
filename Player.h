#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Piece.h"

using namespace std;

class Player {
protected:
	string name;
	string color;
	Piece pieces[4];
	int start_position;

	virtual int roll_dice();
	virtual int choose_piece();
	virtual void handle_six_with_pieces_out(int dice);

	void handle_rolling_to_game(int& dice);
	void handle_six_no_pieces_out(int dice);
	void handle_normal_move(int dice);
public:
	Player(string player_name = "Igrac", string player_color = "Zelena", int start_pos = 0);

	virtual ~Player();

	string get_name() const;
	string get_color() const;

	bool has_piece_out_of_base() const;
	bool all_pieces_in_goal() const;
	int cnt_pieces_in_base() const;
	void show_available_pieces() const;


	virtual void play_turn();
};

#endif
