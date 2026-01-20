#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

constexpr int NUM_PLAYERS = 4;
constexpr int NUM_PIECES_PER_PLAYER = 4;
constexpr int BASE_POSITION = -1;
constexpr int BOARD_SIZE = 40;
constexpr int TOTAL_STEPS_TO_GOAL = 43;
constexpr int HOME_STEPS = 4;
constexpr int MAX_DICE_VALUE = 6;
constexpr int MAX_ATTEMPTS_TO_ROLL = 3;
//kucice moraju bit dalje od ploce
constexpr int HOME_POSITION_BASE = 100;

constexpr int START_POSITIONS[NUM_PLAYERS] = { 0,10,20,30 };
constexpr const char* PLAYER_COLORS[NUM_PLAYERS] = { "Crvena", "Plava", "Zelena","Zuta" };

#endif