#include "Piskvorky.h"
#include "functions.h"

int** arr;
int* popis;
char name1[50], name2[50];
int x, y;
int playerIndex;
int hasMarker[3][3];
int currentMarker;
FILE* lead;

int boardSize()								          // vrací velikost pole
{
	int arrSize;
	printf("Zadaj velkost hracieho pola: \n");	     // user input velikosti pole
	scanf_s("%d", &arrSize);

	                                                 // dynamická alokace herního pole (2D)¨'
	arr = (int**)malloc(arrSize * sizeof(int*));
	for (int i = 0; i < arrSize; i++)
	{
		arr[i] = (int*)malloc(arrSize * sizeof(int));
	};
	if (arr == NULL)				                // kdyby náhodou byl arr pointer NULL i při runtime
	{
		printf("Error: Pointer arr NULL");
		return -1;
	}
	                                                // dynamická alokace pole pro popis souřadnic (1D)
	popis = (int*)calloc(arrSize, sizeof(int));

	                                                 // plnění herního pole mezerama
	for (int i = 0; i < arrSize; i++)
	{
		for (int j = 0; j < arrSize; j++)
		{
			arr[i][j] = ' ';
		}
	}

	system("cls");
	return arrSize;
	
}

void firstPlayer()					// náhodně určí prvního hráče spolu s jeho indexem
{
	srand(time(NULL));
	if (rand() % 2 == 0)
	{
		playerIndex = 1;
		printf("Player %s is starting\nPress enter to continue", name1);
	}
	else
	{
		/*playerIndex = 1;		// vždy budou začínat kolečka
		name_tmp = name1;
		name1 = name2;
		name2 = name_tmp;
		printf("Player %s is starting", name1);*/
		playerIndex = 2;
		printf("Player %s is starting\nPress enter to continue", name2);		// nebudou vždy začínat kolečka
	};
}

void playerNames()					// input jmen hráčů
{
	FILE* names;
	int t = 0;
nameinput:
	printf("1: Enter player names\n2: Load names from previous game\n");		// menu výběru
	scanf_s("%d", &t);
	while (getchar() != '\n');
	system("cls");

	if (t == 1) {																// ruční zadání jmen
		fopen_s(&names, "names.txt", "w+");										// vytvoření souboru pro čtení i zápis
		if (names == NULL)														// kontrola jestli se vytvořil
		{
			printf("Error creating names.txt file");
			return;
		}
		printf("Zadej jmeno a prijmeni: ");
		scanf_s("%[^\n]", name1, 50);
		while (getchar() != '\n');

		printf("Zadej jmeno a prijmeni: ");
		scanf_s("%[^\n]", name2, 50);
		while (getchar() != '\n');

		fprintf(names, "%s, %s", name1, name2);									// zapsání jmen do souboru names.txt
	}
	else if (t == 2) {															// nahrání jmen z minulé hry
		fopen_s(&names, "names.txt", "r+");										// otevření souboru pro čtení i zápis
		if (names == NULL)														// kontrola jestli se otevřel
		{
			printf("Error opening names.txt file");
			return;
		}

		fscanf_s(names, "%[^,],%[^\n]", name1, 50, name2, 50);
	}
	else {																		// kontrola správného výběru
		printf("Please choose a valid option\n");
		goto nameinput;
	}

	fseek(names, 0, SEEK_SET);													// vrácení kurzoru na začátek souboru names.txt
	fclose(names);

}

void drawBoard(int arrSize)					// vykresluje herní pole
{
	printf("    ||");					// odsazení popisků sloupců

	for (int count = 0; count < arrSize; count++)		// printing popisků sloupců
	{
		popis[count] = count + 1;
		if (popis[count] >= 10)
		{
			printf("%d|", popis[count]);
		}
		else
		printf("%d |", popis[count]);
	}
	printf("\n");
	for (int i = 0; i < arrSize; i++)
	{
		if (i == 0)
		{
			printf("    ||");
		}
		else
		printf("====");
	}
	printf("\n");
	/*for (i = 0; i < r; i++)
	{
		for (j = 0; j < c; j++)			plnění jedničkama pro testing
		{
			arr[i][j] = 1;
		};
	};*/

	for (int i = 0; i < arrSize; i++)			// printing popisků řádků a herního pole
	{
		popis[i] = i + 1;				// popisky řádků
		if (popis[i] >= 10)
		{
			printf(" %d ||", popis[i]);
		}
		else
			printf(" %d  ||", popis[i]);	

		for (int j = 0; j < arrSize; j++)			// herní pole
		{
			printf("%c |", arr[i][j]);
		};
		printf("\n");
	};
}

void placeMarker(int arrSize)
{
	// přiřadí jménům index a marker
	if (playerIndex == 1)
	{
		printf("Its %s s turn\n", name1);
		currentMarker = 'O';
	}
	else if (playerIndex == 2)
	{
		printf("Its %s s turn\n", name2);
		currentMarker = 'X';
	}
	else
	{
		printf("it brokey lmao\n"); // pokud je playerIndex neočekávaná hodnota
	};

	// user input polohy markeru
x_input:
	printf("Zadaj x: ");
	scanf_s("%d", &y);		
	while (getchar() != '\n');              // pošéfit aby nešel zadat charakter místo čísla
	if (y < 1 || y > arrSize)				// kontrola zda je X v poli
	{
		printf("Input valid position\n");
		goto x_input;
	};

y_input:
	printf("Zadaj y: ");
	scanf_s("%d", &x);
	while (getchar() != '\n');
	if (x < 1 || x > arrSize)				// kontrola zda je Y v poli
	{
		printf("Input valid position\n");
		goto y_input;
	};

	// kontrola jestli na zadaných souřadnicích už není marker
	int overlap = overlapCheck();

	if (overlap == 1)
	{
		printf("There's already a marker there\n");
		goto x_input;
	}
	else
	{
		arr[x - 1][y - 1] = currentMarker;
		hasMarker[x - 1][y - 1] = 1;
	}

	// změní playerIndex
	
	currentPlayer();

}

int overlapCheck()					// kontrola jestli na zadaných souřadnicích už není marker
{
	int overlap;

	if (arr[x - 1][y - 1] != 32)
	{
		overlap = 1;
	}
	else
	{
		overlap = 0;
	}
	return overlap;
}

void currentPlayer()				// mění playerIndex, tzn hráče na tahu
{
	if (playerIndex == 1)
	{
		playerIndex += 1;
	}
	else
	{
		playerIndex -= 1;
	}
}

int checkWin(int arrSize)
{

	for (int i = 0; i <= (arrSize - 3); i++)
		{
			for (int j = 0; j <= (arrSize - 3); j++)
			{
				int arrCheck[3][3] =
				{ arr[i][j], arr[i][j+1], arr[i][j+2],
				arr[i+1][j],arr[i + 1][j + 1],arr[i + 1][j + 2],
				arr[i+2][j], arr[i + 2][j + 1], arr[i + 2][j + 2] };

				//check rows
				for (int k = 0; k < 3; k++)
				{
					if (arrCheck[k][0] == arrCheck[k][1] && arrCheck[k][0] == arrCheck[k][2] && arrCheck[k][0] != 32)
					{
						return arrCheck[k][0];
					}
				}
				//check columns
				for (int k = 0; k < 3; k++)
				{
					if (arrCheck[0][k] == arrCheck[1][k] && arrCheck[0][k] == arrCheck[2][k] && arrCheck[0][k] != 32)
					{
						return arrCheck[0][k];
					}
				}
				//check diagonals
				if (arrCheck[0][0] == arrCheck[1][1] && arrCheck[0][0] == arrCheck[2][2] && arrCheck[0][0] != 32)
				{
					return arrCheck[0][0];
				}
				if (arrCheck[0][2] == arrCheck[1][1] && arrCheck[0][2] == arrCheck[2][0] && arrCheck[0][2] != 32)
				{
					return arrCheck[0][2];
				}

			
			}

		}

		return 0;
}

void printWinner(int winner)
{
	if (winner == 79)							// 79 je O v ASCII 
	{
		printf("Winner is %s", name1);			// na začátku se name1 vždy přiřadí kolečko
		while (getchar() != '\n');
	}
	else										// jiná hodnota než 88 (X v ASCII nemůže nastat)
	{
		printf("Winner is %s", name2);			// name2 má vždy křížek
		while (getchar() != '\n');
	}
}



void leaderboard(int winner) {
	FILE* lead;
	fopen_s(&lead, "leaderboard.txt", "r+");
	if (lead == NULL) {
		fopen_s(&lead, "leaderboard.txt", "w+");
	}
	switch (winner == 79)
	{
	case 1:
		char winplayer[50] = *name1;
			break;
	}
	int count = 0;
	fprintf_s(lead, "%s\n", winplayer);
}