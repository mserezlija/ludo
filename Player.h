#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Piece.h"
#include "GameConstants.h"

using namespace std;

class Board;
class Game;

class Player {
protected:

	class Game* game_ref = nullptr;

	string name;
	string color;
	Piece pieces[NUM_PIECES_PER_PLAYER];
	bool home_occupied[HOME_STEPS];
	int start_position;
	int last_move_pos = -1;

	virtual int roll_dice() = 0;
	virtual int choose_piece(int dice) = 0;
	virtual bool new_or_move() = 0;

	bool execute_move(int dice, Board* board);

	void handle_rolling_to_game(int& dice);
	void handle_six(int dice, Board* board);
	void handle_normal_move(int dice, Board* board);
	void take_piece_from_base(Board* board);

	int find_available_home(int needed_steps);
public:
	Player(const string& player_name, const string& player_color, int start_pos);

	virtual ~Player();

	void set_game(Game* g);

	string get_name() const;
	string get_color() const;
	int get_start_pos() const;
	Piece* get_piece(int index);
	int get_last_move_position() const;

	bool has_piece_on_board() const;
	bool all_pieces_in_goal() const;
	int cnt_pieces_in_base() const;
	int cnt_pieces_in_goal() const;
	bool has_valid_move(int dice) const;

	virtual void play_turn(Board* board);
};

#endif
