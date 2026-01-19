#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

class GameConstants {
public:
	static constexpr int NUM_PLAYERS = 4;
	static constexpr int NUM_PIECES_PER_PLAYER = 4;
	static constexpr int BASE_POSITION = -1;
	static constexpr int BOARD_SIZE = 40;
	static constexpr int TOTAL_STEPS_TO_GOAL = 44;
	static constexpr int HOME_STEPS = 4;
	static constexpr int MAX_DICE_VALUE = 6;
	static constexpr int MAX_ATTEMPTS_TO_ROLL = 3;
	//kucice moraju bit dalje od ploce
	static constexpr int HOME_POSITION_BASE = 100;

	static constexpr int START_POSITIONS[NUM_PLAYERS] = { 0,10,20,30 };
	static constexpr const char* PLAYER_COLORS[NUM_PLAYERS] = { "Crvena", "Plava", "Zelena","Zuta" };
	
};

#endif