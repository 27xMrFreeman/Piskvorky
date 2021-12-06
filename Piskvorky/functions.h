#define NAME_SIZE 20
#define SIZE 10
int boardSize();
void playerNames();
int firstPlayer();
void drawBoard(int arrSize);
int placeMarker(int arrSize,int playerIndex1);
int overlapCheck(int x, int y);
int currentPlayer(int playerIndex);
int checkWin(int arrSize);
void printWinner(int winner);
int gameOver();
void leaderboard();
void printLead();