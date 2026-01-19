#include <iostream>
#include "Board.h"
#include "Game.h"
#include "Player.h"
#include "GameConstants.h"

Board::Board(Game* g) :game(g) {
	for (int i = 0; i < BOARD_SIZE; i++) { board_fields[i] = nullptr; }
}

bool Board::is_avail(int pos) const {
	if (pos < 0 || pos >= BOARD_SIZE) { return false; }
	return board_fields[pos] == nullptr;
}

void Board::place(Piece* piece, int pos) {
	if (pos >= 0 && pos < BOARD_SIZE) { board_fields[pos] = piece; }
}

void Board::clear_pos(int pos) {
	if (pos >= 0 && pos < BOARD_SIZE) { board_fields[pos] = nullptr; }
}

bool Board::eat_enemy(Piece* my_piece, int pos) {
    if (!my_piece || my_piece->is_in_base() || my_piece->is_in_goal()) { return false; }

    if (pos < 0 || pos >= BOARD_SIZE) { return false; }

    bool eaten = false;
    int my_start = my_piece->get_owner_start();

    for (int i = 0; i < NUM_PLAYERS; i++) {
        Player* p = game->get_player(i);
        if (!p) continue;

        if (p->get_start_pos() == my_start) continue;

        for (int j = 0; j < NUM_PIECES_PER_PLAYER; j++) {
            Piece* enemy = p->get_piece(j);
            if (!enemy) continue;

            if (enemy->get_position() == pos &&
                !enemy->is_in_base() &&
                !enemy->is_in_goal() &&
                !enemy->is_in_home()) {

                enemy->return_to_base();
                cout << p->get_color() << " figura je pojedena!" << endl;
                eaten = true;
            }
        }
    }

    return eaten;
}

Piece* Board::get_piece_at(int pos) const {
	if (pos >= 0 && pos < BOARD_SIZE) {
		return board_fields[pos];
	}
	return nullptr;
}