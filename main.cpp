#include <iostream>
#include "Game.h"
#include "Graphics.h"

using namespace std;

int main() {
    
	cout << "Koliko ce biti ljudskih igraca: " << endl;
	int num;
	cin >> num;
	cin.ignore();

	if (num < 1 || num > 4) num = 1;

	Game igra(num);
	Graphics grafika(&igra);
	igra.set_graphics(&grafika);

	igra.start_game();

	return 0;
}
