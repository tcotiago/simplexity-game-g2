#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Constants */
/** Horizontal board size. */
#define MAX_ROW 7
/** Vertical board size. */
#define MAX_COL 7
/** Game pieces O and X */
#define PIECES "OX"


char board[MAX_ROW][MAX_COL]; // Array 2D Board

void clearBoard(){
	for(int r = 0; r < MAX_ROW ; r++) // Go through all rows
		for(int c = 0; c < MAX_COL ; c++) // Go through all cols
			board[r][c] = 0; // Reset all rows and cols
	//playerTurn = 0;
}

char* getPrintableSymbol(char c){
	if(c == 4) return "\x1B[97m0\x1B[0m"; // White O
	if(c == 5) return "\x1B[31m0\x1B[0m"; // Red O
	if(c == 6) return "\x1B[97mX\x1B[0m"; // White X
	if(c == 7) return "\x1B[31mX\x1B[0m"; // Red X
	
	//if(c == 4) return "\x1B[97m"+PIECES[0]+"\x1B[0m"; // White O
	//if(c == 5) return "\x1B[31m"+PIECES[0]+"\x1B[0m"; // Red O
	//if(c == 6) return "\x1B[97m"+PIECES[1]+"\x1B[0m"; // White X
	//if(c == 7) return "\x1B[31m"+PIECES[1]+"\x1B[0m"; // Red X
	
	else return "\x1B[90m|\x1B[0m"; // Else |
}



void viewBoard(){
	for(int r = MAX_ROW -1; r >= 0 ; r--){ // From top to bottom
		for(int c = 0; c < MAX_COL ; c++)  // Cols from left to right
			printf(" %s ", getPrintableSymbol(board[r][c])); // Print as string
		printf("\n");
	}
	printf("\x1B[90m");
	for(int c = 0; c < MAX_COL ; c++) // Base
		printf("---");
	printf("\n");
	for(int c = 0; c < MAX_COL ; c++) // Col Nrs
		printf(" %d ", c+1);
	printf("\n\n\x1B[0m");
}

int getAvailableRow(int col){
	for (int r = 0; r < MAX_ROW; r++)
		if(board[r][col] == 0)
			return r;
	return -1; // When row is full return false
}

int getPieceId (int symbol, int turn){
	// Codifica o simbolo em valor p/ o tabuleiro
	
	return 4+symbol*2 + turn; // De 4 a 7
}

char getPiece (int row, int col){
	
	return board[row][col];
}

int getColor (int row, int col){
	
	return getPiece(row, col)%2;
}

int getSymbol (int row, int col){
	
	char piece = getPiece(row, col);
	if(piece==0)
		return 0;
	else return (piece>>1)%2 +1;
}

bool insertPiece(int col, int symbol, int turn){
	
	if( col < 0 || col >= MAX_COL ) // Error handler input column
		return false;
	int row = getAvailableRow(col); // Stack on rows
	if(row >= 0){ // Error handler full
		board[row][col] = getPieceId(symbol, turn);
		return true;
	}
	else return false;
}

///////////////// NEW FILE (HMI's)
int playerTurn = 0;
int turnCounter = 1;

int nextTurn(){ // Advances turn and changes player (P0 and P1)
	playerTurn = (playerTurn + 1)%2;
	return turnCounter += 1;
}

int askTurnChoices(){
	int col_sel;
	int piece_sel;
	bool nok;
	do { 
		if(playerTurn == 0){
			printf("Player 1's turn \n");
		}
		else printf("Player 2's turn \n");
		do {
			printf("Turn number: %d \n", turnCounter);
			printf("Choose a valid column: \t");
			scanf("%d", &col_sel);
			nok = (col_sel < 1) || (col_sel > MAX_COL);
			if(nok)
				printf("You selected an invalid column.\n\n");
		} while (nok);
		do {
			printf("Choose a valid piece type (1 for %c or 2 for %c): ", PIECES[0], PIECES[1]);
			scanf("%d", &piece_sel);
			nok = (piece_sel < 1) || (piece_sel > 2);
			if(nok)
				printf("You selected an invalid piece type.\n\n");
		} while (nok);
		nok = !insertPiece(col_sel -1, piece_sel -1, playerTurn);
		if(nok)
			printf("The selected column is full.\n\n");
	} while (nok);
	
	nextTurn(); // Advance 1 turn
	viewBoard(); // Render Board
	return 0;
}


/////////////////

int main() {
	clearBoard();
	
	// Debugging manual insert
	//insertPiece(0, 0, 0);
	//insertPiece(0, 1, 1);
	//insertPiece(0, 0, 1);
	//insertPiece(0, 1, 0);
	//insertPiece(0, 1, 0);	
	viewBoard(); // Remover ao organizar interaccao
	//////////////////////////
	
	askTurnChoices();
	askTurnChoices();
	askTurnChoices();
	askTurnChoices();
	
	printf("Simbolo %c, cor %d, \n", PIECES[getSymbol(0, 0)-1], getColor(0, 0));
	printf("Simbolo %c, cor %d, \n", PIECES[getSymbol(1, 0)-1], getColor(1, 0));
	printf("Simbolo %c, cor %d, \n", PIECES[getSymbol(2, 0)-1], getColor(2, 0));
	//board[0][0] = 4;
	//board[6][6] = 7;
	
	
	return 0;	
}

