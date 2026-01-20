#include <iostream>
#include "Game.h"
#include "Graphics.h"

using namespace std;

int main() {
    
	/*cout << "Koliko ce biti ljudskih igraca: " << endl;
	int num;
	cin >> num;
	cin.ignore();

	if (num < 1 || num > 4) num = 1;*/

	Graphics temp_graph(nullptr);
	int num = temp_graph.wait_for_player_cnt();

	Game igra(num, &temp_graph);;
	//Graphics grafika(&igra);
	//igra.set_graphics(&grafika);

	igra.start_game();

	return 0;
}
