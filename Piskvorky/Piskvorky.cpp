// Piskvorky.cpp : Defines the entry point for the application.
//

#include "Piskvorky.h"
#include "functions.h"

using namespace std;

int main()
{
	
	int arrSize = boardSize();
	
	playerNames();

	firstPlayer();                        //toto by som prerobila na int funkciu - budeme mat o jednu globalnu premennu menej

	//getchar();


	for (int i = 0; i < arrSize*arrSize; i++)
	{
		system("cls");

		drawBoard(arrSize);

		placeMarker(arrSize);

		int winner = checkWin(arrSize);
		
		if (winner != 0)
		{
			system("cls");
			drawBoard(arrSize);
			printWinner(winner);
			// updateLeaderboard
		}
		while (getchar() != '\n');

	}

	return 0;
}
