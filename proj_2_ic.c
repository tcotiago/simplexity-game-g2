#include <stdio.h>
#include <stdlib.h>

/* Constants */
/** Horizontal board size. */
#define MAX_ROW 7
/** Vertical board size. */
#define MAX_COL 7

#define PIECES "OX"

char board[MAX_ROW][MAX_COL]; // Array 2D Board

void clearBoard(){
	for(int r = 0; r < MAX_ROW ; r++)
		for(int c = 0; c < MAX_COL ; c++)
			board[r][c] = 0;
}

char* getSymbol(char c){
	if(c == 4) return "\e[97mO"; // White O
	if(c == 5) return "\e[31mO"; // Red O
	if(c == 6) return "\e[97mX"; // White X
	if(c == 7) return "\e[31mX"; // Red X
	return "\e[90m|";
}

void viewBoard(){
	for(int r = MAX_ROW -1; r >= 0 ; r--){
		for(int c = 0; c < MAX_COL ; c++)
			printf(" %s ", getSymbol(board[r][c]));
		
		printf("\n");
	}
	for(int c = 0; c < MAX_COL ; c++) // Base
		printf("---");
	printf("\n");
	for(int c = 0; c < MAX_COL ; c++) // Col Nrs
		printf(" %d ", c+1);
	printf("\n");
}

int getAvailableRow(int col){
	
	for (int r = 0; r < MAX_ROW; r++)
		if(board[r][col] == 0)
			return r;
	return -1;	
	
}

int insertPiece(int col, int symbol, int color){
	if( col < 0 || col >= MAX_COL )
		return 0;
	int piece = 4+symbol*2 + color; // De 4 a 7
	int row = getAvailableRow(col);
	if(row >= 0){
		board[row][col] = piece;
		return 1; // true
	}
	return 0;
}

int main(int argc, char *argv[]) {
	clearBoard();
	insertPiece(0, 0, 0);
	insertPiece(0, 1, 1);
	insertPiece(0, 0, 1);
	insertPiece(5, 1, 0);
	//board[0][0] = 4;
	//board[6][6] = 7;
	viewBoard();
	
	return 0;	
}

