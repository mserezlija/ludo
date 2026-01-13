#include <iostream>
#include "Game.h"

using namespace std;

int main() {
	cout << "koliko ce biti ljudskih igraca: " << endl;
	int num;
	cin >> num;
	cin.ignore();

	if (num < 1 || num>4) num = 1;

	Game igra(num);
	igra.start_game();


	return 0;
}