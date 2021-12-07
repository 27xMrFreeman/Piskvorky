#include "Piskvorky.h"
#include "functions.h"

int** arr;
int* popis;
FILE* lead;

struct t_score{
	char name [NAME_SIZE];
	int win = 0;
}player1,player2;

int index[SIZE];
typedef struct t_leaderBoard {
	char name[NAME_SIZE];
	int win;
}player;

int boardSize()								          // vrací velikost pole
{
	start:
	int arrSize;
	printf("Choose board size: \n");	     // user input velikosti pole
	scanf_s("%d", &arrSize);
	system("cls");
	if (arrSize < 3)
	{
		printf("Please choose a valid option (Minimal size is 3)\n");
		goto start;
	}
	                                                 // dynamická alokace herního pole (2D)¨'
	arr = (int**)malloc(arrSize * sizeof(int*));
	for (int i = 0; i < arrSize; i++)
	{
		arr[i] = (int*)malloc(arrSize * sizeof(int));
	}
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

int firstPlayer()					// náhodně určí prvního hráče spolu s jeho indexem
{
	int playerIndex;
	srand(time(NULL));
	if (rand() % 2 == 0)
	{
		playerIndex = 1;
		printf("Player %s is starting\n", player1.name);
		while (getchar() != '\n');
	}
	else
	{
		playerIndex = 2;
		printf("Player %s is starting\n", player2.name);	// nebudou vždy začínat kolečka
		while (getchar() != '\n');
	}
	return playerIndex;
}

void playerNames()																// input jmen hráčů
{
	FILE* names;
	int t = 0;
nameinput:
	printf("1: Enter player names\n2: Load names from previous game\n");		// menu výběru
	scanf_s("%d", &t);
	while (getchar() != '\n');
	system("cls");

	if (t == 1) 
	{																			// ruční zadání jmen
		fopen_s(&names, "names.txt", "w+");										// vytvoření souboru pro čtení i zápis
		if (names == NULL)														// kontrola jestli se vytvořil
		{
			printf("Error creating names.txt file");
			return;
		}
		printf("Choose first name: ");
		scanf_s("%[^\n]", player1.name, 20);
		while (getchar() != '\n');

		printf("Choose second name: ");
		scanf_s("%[^\n]", player2.name, 20);
		while (getchar() != '\n');

		fprintf(names, "%s,%s", player1.name, player2.name);					// zapsání jmen do souboru names.txt
	}

	else if (t == 2) 
	{																			// nahrání jmen z minulé hry
		fopen_s(&names, "names.txt", "r+");										// otevření souboru pro čtení i zápis
		if (names == NULL)														// kontrola jestli se otevřel
		{
			printf("Error opening names.txt file");
			return;
		}

		fscanf_s(names, "%[^,],%[^\n]", player1.name, NAME_SIZE, player2.name, NAME_SIZE);
	}

	else 
	{																			// kontrola správného výběru
		printf("Please choose a valid option\n");	
		goto nameinput;
	}

	fseek(names, 0, SEEK_SET);													// vrácení kurzoru na začátek souboru names.txt
	fclose(names);

}

void drawBoard(int arrSize)					                       // vykresluje herní pole
{
	printf("    ||");					                           // odsazení popisků sloupců

	for (int count = 0; count < arrSize; count++)		           // printing popisků sloupců
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

int placeMarker(int arrSize,int playerIndex1)
{			
	int x, y;
	int currentMarker;
	if (playerIndex1 == 1)
	{
		printf("Its %s s turn\n", player1.name);
		currentMarker = 'O';
	}
	else if (playerIndex1 == 2)
	{
		printf("Its %s s turn\n", player2.name);
		currentMarker = 'X';
	}
	else
	{
		printf("it brokey lmao\n");			// pokud je playerIndex neočekávaná hodnota
	}
											// user input polohy markeru
x_input:
	printf("Zadaj x: ");
	scanf_s(" %d", &y);	
	while (getchar() != '\n');              
	if (y < 1 || y > arrSize)				// kontrola zda je X v poli
	{
		printf("Input valid position\n");
		goto x_input;
	};

y_input:
	printf("Zadaj y: ");
	scanf_s(" %d", &x);
	while (getchar() != '\n');
	if (x < 1 || x > arrSize)				// kontrola zda je Y v poli
	{
		printf("Input valid position\n");
		goto y_input;
	};

											// kontrola jestli na zadaných souřadnicích už není marker
	int overlap = overlapCheck(x,y);

	if (overlap == 1)
	{
		printf("There's already a marker there\n");
		goto x_input;
	}
	else
	{
		int hasMarker[3][3];
		arr[x - 1][y - 1] = currentMarker;
		hasMarker[x - 1][y - 1] = 1;
	}
	return 0;
 }

int overlapCheck(int x, int y)					// kontrola jestli na zadaných souřadnicích už není marker
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

int currentPlayer(int playerIndex)				// mění playerIndex, tzn hráče na tahu
{
	if (playerIndex == 1)
	{
		playerIndex += 1;
	}
	else
	{
		playerIndex -= 1;
	}
	return playerIndex;
}												// změní playerIndex

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
	fopen_s(&lead, "leaderboard.txt", "a+");
	if (lead == NULL) { fopen_s(&lead, "leaderboard.txt", "w+"); }
	if (winner == 79)										// 79 je O v ASCII 
	{
		printf("\n   Winner is %s", player1.name);			// na začátku se name1 vždy přiřadí kolečko
		while (getchar() != '\n');
		player1.win = ++player1.win;
		fprintf(lead, "%s %d\n", player1.name, player1.win);
		player1.win = 0;
	}
	else													// jiná hodnota než 88 (X v ASCII nemůže nastat)
	{
		printf("\n   Winner is %s", player2.name);			// name2 má vždy křížek
		while (getchar() != '\n');
		player2.win = ++player2.win;
		fprintf(lead, "%s %d\n", player2.name, player2.win);
		player2.win = 0;
	}
	fclose(lead);
}

int gameOver()
{
nameinput:
	int t;
	system("cls");
	printf("1: Quit game\n2: Restart game\n3: Show leaderboard\n");				// menu výběru
	scanf_s("%d", &t);
	while (getchar() != '\n');
	if (t == 3)
	{
		system("cls");
		printLead();
		goto nameinput;
	}
	if (t != 1 && t != 2 && t !=3)
	{														// kontrola správného výběru
		goto nameinput;
	}
	system("cls");
	return t;
}

void leaderboard() {
	player p[SIZE] = {};
	fopen_s(&lead, "leaderboard.txt", "a+");								// otevření souboru pro připisování i čtení
	if (lead == NULL) {														// kontrola jestli se otevřel
		fopen_s(&lead, "leaderboard.txt", "w+");							// vytvoření souboru pro čtení i zápis
		if (lead == NULL) {													// kontrola jestli se otevřel
			printf("Error creating leaderboard.txt file");
			return;
		}
	}
	fprintf(lead,"%s %d\n", player1.name, player1.win);
	fprintf(lead,"%s %d\n", player2.name, player2.win);

	/*for (int i = 0; i < SIZE && !feof(lead); i++)
	{
		/*if (strcmp(p[i].name, player1.name) != 0) {
			fopen_s(&lead, "leaderboard.txt", "r+");
			fprintf(lead, "%s %d", player1.name, player1.win);
			fclose(lead);
		}
		else if (strcmp(p[i].name, player2.name) != 0){
			fopen_s(&lead, "leaderboard.txt", "r+");
			fprintf(lead, " %s %d", player2.name, player2.win);
			fclose(lead);
		}
		else {
			fopen_s(&lead, "leaderboard.txt", "a+");
			fprintf(lead, "%s %d", player1.name, player1.win);
			fprintf(lead, " %s %d", player2.name, player2.win);
			fclose(lead);
		}
	}*/
	
	fclose(lead);
}

void printLead() 
{
	/*
	if (player1.win != 0)
	{
		printf("%s %d\n", player1.name, player1.win);
	}
	if (player2.win != 0)
	{
		printf("%s %d\n", player2.name, player2.win);
		while (getchar() != '\n');
	}
	while (getchar() != '\n');
	*/

	int howMany = 0;
	player temp;
	player p[SIZE] = {};
	fopen_s(&lead, "leaderboard.txt", "r+");
	if (lead == NULL) { printf("leaderboard.txt is NULL"); }
	for (int i = 0; i < SIZE && !feof(lead); i++)
	{
		fscanf(lead, "%s %d%*c", p[i].name, &p[i].win);
		howMany++;
	}
	int i, j;
	for (i = 0; i < howMany; i++)
	{
		for (j = 0; j < howMany - 1; j++)
		{
			if (p[j].win < p[j + 1].win)
			{
				temp = p[j];
				p[j] = p[j + 1];
				p[j + 1] = temp;
			}
			if (strcmp(p[j].name, p[j + 1].name) == 0) {
				p[j].win = p[j].win + p[j + 1].win;
				p[j + 1].win = 0;
			}
		}
	}


	for (int i = 0; i < howMany-1; i++)
	{
		if (p[i].win != 0) {
			if (i <= 9) {
				printf("%s %d\n", p[i].name, p[i].win);
			}
		}
	}
	fclose(lead);
	while (getchar() != '\n');

}    