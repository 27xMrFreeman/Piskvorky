// Piskvorky.cpp : Defines the entry point for the application.
//

#include "Piskvorky.h"
#include "functions.h"

using namespace std;

int main()
{
	srand(time(NULL));

	boardSize();
	playerNames();
	firstPlayer();

	//getchar();


	for (int i = 0; i < arrSize*arrSize; i++)
	{
		system("cls");

		drawBoard();

		placeMarker();

		int winner = checkWin();
	
		printWinner(winner);

		while (getchar() != '\n');

	}

	return 0;
}
