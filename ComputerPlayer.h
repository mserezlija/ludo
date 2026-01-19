#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H
#include "Player.h"

class ComputerPlayer : public Player {
private:
	int roll_dice() override;
	int choose_piece(int dice) override;
	bool new_or_move() override;
public:
	ComputerPlayer(string player_name, string player_color, int start_pos);
};

#endif
