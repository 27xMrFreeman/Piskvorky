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


	for (int i = 0; i < 9; i++)
	{
		system("cls");

		drawBoard();

		placeMarker();

		//checkWin;


	}

	return 0;
}
