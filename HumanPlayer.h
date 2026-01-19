#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "Player.h"

class HumanPlayer : public Player {
protected:
	int roll_dice() override;
	int choose_piece(int dice) override;
	bool new_or_move() override;
public:
	HumanPlayer(const string& name, const string& color, int start_pos);
};

#endif
