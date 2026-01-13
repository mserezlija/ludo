#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H
#include "Player.h"

class ComputerPlayer : public Player {
private:
	int roll_dice() override;
	int choose_piece() override;
	void handle_six_with_pieces_out(int dice) override;
public:
	ComputerPlayer(string player_name, string player_color, int start_pos);

	void play_turn() override;
};

#endif