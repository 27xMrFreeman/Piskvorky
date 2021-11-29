#include "Piskvorky.h"
#include "functions.h"

char** arr;
double arrSize;
int* popis;
int r, c;
char name1[50];
char name2[50];
char name_tmp[100];
int x;
int y;
int i = 0, j = 0;
int playerIndex;
int overlap;
int hasMarker[3][3];
int currentMarker;
FILE* names, lead;

void boardSize()
{

	printf("Enter array size: \n");	// user input velikosti pole
	scanf_s("%lf", &arrSize);
	r = arrSize;
	c = arrSize;

	// dynamická alokace herního pole (2D)
	arr = (char**)malloc(100 * sizeof(char*));
	for (i = 0; i < r; i++)
	{
		arr[i] = (char*)malloc(100 * sizeof(char));
	};
	if (arr == NULL)				// kdyby náhodou byl arr pointer NULL i při runtime
	{
		printf("Error: Pointer arr NULL");
		return;
	}
	// dynamická alokace pole pro popis souřadnic (1D)
	popis = (int*)calloc(arrSize, sizeof(int));

	// plnění herního pole mezerama
	for (i = 0; i < arrSize; i++)
	{
		for (j = 0; j < arrSize; j++)
		{
			arr[i][j] = ' ';
		}
	}

	while (getchar() != '\n');
	system("cls");
}

void firstPlayer()					// náhodně určí prvního hráče spolu s jeho indexem
{
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
	fopen_s(&names, "names.txt", "r+");			// otevření souboru se jmény, soubor musí existovat
	if (names == NULL)							// kontrola jestli se otevřel
	{
		printf("Error opening Names file");
		return;
	}

	int t = 0;
	printf("1: Enter player names\n2: Load names from previous game\n");		// menu výběru
	nameinput:
	scanf_s("%d", &t);
	while (getchar() != '\n');
	system("cls");

	if (t == 1) {																// ruční zadání jmen
		printf("Zadej jmeno a prijmeni: ");
		scanf_s("%[^\n]s", name1, 50);
		while (getchar() != '\n');

		printf("Zadej jmeno a prijmeni: ");
		scanf_s("%[^\n]s", name2, 50);
		while (getchar() != '\n');

		fprintf(names, "%s\n%s", name1, name2);									// zapsání jmen do souboru names.txt
	}
	else if (t == 2) {															// nahrání jmen z minulé hry
		fscanf_s(names, "%[^\n]s%[^\n]s", name1, 50, name2, 50);
	}
	else {																		// kontrola správného výběru
		printf("Choose valid option");
		goto nameinput;
	}

	fseek(names, 0, SEEK_SET);													// vrácení kurzoru na začátek souboru names.txt
	fclose(names);

}

void drawBoard()					// vykresluje herní pole
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
	for (i = 0; i < arrSize; i++)
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

	for (i = 0; i < r; i++)			// printing popisků řádků a herního pole
	{
		popis[i] = i + 1;				// popisky řádků
		if (popis[i] >= 10)
		{
			printf(" %d ||", popis[i]);
		}
		else
			printf(" %d  ||", popis[i]);	

		for (j = 0; j < c; j++)			// herní pole
		{
			printf("%c |", arr[i][j]);
		};
		printf("\n");
	};
}

void placeMarker()
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
	scanf_s("%d", &y);				// pošéfit aby nešel zadat charakter místo čísla
	if (y < 1 || y > arrSize)				// kontrola zda je X v poli
	{
		printf("Input valid position\n");
		goto x_input;
	};

y_input:
	printf("Zadaj y: ");
	scanf_s("%d", &x);

	if (x < 1 || x > arrSize)				// kontrola zda je Y v poli
	{
		printf("Input valid position\n");
		goto y_input;
	};

	// kontrola jestli na zadaných souřadnicích už není marker
	overlapCheck();

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
	//checkWin();
	currentPlayer();

}

void overlapCheck()					// kontrola jestli na zadaných souřadnicích už není marker
{
	if (arr[x - 1][y - 1] == 'X' || arr[x - 1][y - 1] == 'O')
	{
		overlap = 1;
	}
	else
	{
		overlap = 0;
	}

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

int checkWin()
{

	for (i = 0; i <= (arrSize - 3); i++)
		{
			for (j = 0; j <= (arrSize - 3); j++)
			{
				char arrCheck[3][3] =
				{ arr[i][j], arr[i][j+1], arr[i][j+2],
				arr[i+1][j],arr[i + 1][j + 1],arr[i + 1][j + 2],
				arr[i+2][j], arr[i + 2][j + 1], arr[i + 2][j + 2] };

				//check rows
				for (int k = 0; k < 3; k++)
				{
					if (arrCheck[k][0] == arrCheck[k][1] && arrCheck[k][0] == arrCheck[k][2])
					{
						return arrCheck[k][0];
					}
				}
				//check columns
				for (int k = 0; k < 3; k++)
				{
					if (arrCheck[0][k] == arrCheck[1][k] && arrCheck[0][k] == arrCheck[2][k])
					{
						return arrCheck[0][k];
					}
				}
				//check diagonals
				if (arrCheck[0][0] == arrCheck[1][1] && arrCheck[0][0] == arrCheck[2][2])
				{
					return arrCheck[0][0];
				}
				if (arrCheck[0][2] == arrCheck[1][1] && arrCheck[0][2] == arrCheck[2][0])
				{
					return arrCheck[0][2];
				}

			
			}

		}

		return 1;
}

void printWinner(int winner)
{
	printf("Winner is %d", winner);
	while (getchar() != '\n');
}