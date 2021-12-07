// Piskvorky.cpp : Defines the entry point for the application.
//

#include "Piskvorky.h"
#include "functions.h"

using namespace std;

int main()
{
	printf("TIC-TAC-TOE\n");
Start:
	playerNames();
	int arrSize = boardSize();
	int playerIndex = firstPlayer();                       
	for (int i = 0; i < arrSize*arrSize; i++)
	{
		system("cls");
		drawBoard(arrSize);
		int currentMarker = placeMarker(arrSize,playerIndex);
		playerIndex = currentPlayer(playerIndex);
		int winner = checkWin(arrSize);
		if (winner != 0)
		{
			system("cls");
			drawBoard(arrSize);
			printWinner(winner);
			int x = gameOver();
			if (x == 2)
			{
				goto Start;
			}
			 
			return 0;
		}
	}
	printf("Game over - DRAW");
	while (getchar() != '\n');
	int x = gameOver();
	if (x == 2)
	{
		goto Start;
	}
	return 0;
}
