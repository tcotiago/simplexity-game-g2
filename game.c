#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "constants.h"
#include "game.h"
#include "validation.h"
#include "interface.h"


char board[MAX_ROW][MAX_COL]; // Char Array 2D - Board

// Function to clear Board/Array
void clearBoard(){
	for(int r = 0; r < MAX_ROW ; r++) // Go through all rows
		for(int c = 0; c < MAX_COL ; c++) // Go through all cols
			board[r][c] = 0; // Reset all rows and cols
}

// Returns printable char in correct color for terminal
char* getPrintableSymbol(char c){ 
	// Changes color, prints 0 or X, and reset attributes
	if(c == 4) return "\x1B[97m0\x1B[0m"; // White Cylinder
	if(c == 5) return "\x1B[31m0\x1B[0m"; // Red Cylinder
	if(c == 6) return "\x1B[97mX\x1B[0m"; // White Square
	if(c == 7) return "\x1B[31mX\x1B[0m"; // Red Square
	
	else return "\x1B[90m|\x1B[0m"; // Else |
}


// Function to stack on cols
int getAvailableRow(int col){
	for (int r = 0; r < MAX_ROW; r++)
		if(board[r][col] == 0) // If in this position there is no piece
			return r; // Returns available row
	return -1; // When row is full return false
}

// Codifies symbol for board
int getPieceId (int symbol, int turn){
	
	return 4+symbol*2 + turn; // De 4 a 7
	// 4 + 0-1 * 2 + 0/1
	
	// 4 White O 
	// 5 White X
	// 6 Red O
	// 7 Red X
}

// Function that returns what is in a given position
char getPiece (int row, int col){ // Asks for cell on Array
	return board[row][col]; // Returns a character from the 2D Array
}

// Returns the player turn according to color in a given position
int getTurn (int row, int col){ // Get color
	char piece = getPiece(row, col);
	if(piece==0) // If there is no piece
		return 0; // Return Zero
	else return piece%2 + 1; // If there is a piece, return color
}

// Returns symbol in a given position
int getSymbol (int row, int col){
	char piece = getPiece(row, col);
	if(piece==0) // Empty
		return 0;
	else return (piece>>1) - (sizeof(PIECES)-1) + 1; // Size of 0X (2)
}

// Function to print Ascii based Board, according to defines (constants.h)
void viewBoard(){
	for(int r = MAX_ROW -1; r >= 0 ; r--){ // Rows from top to bottom
		for(int c = 0; c < MAX_COL ; c++){  // Cols from left to right
			printf(" %s ", getPrintableSymbol(board[r][c])); // Print symbol as string
		}
		printf("\n");
	}
	// Print the bottom (base) and the column numbers
	printf("\x1B[90m"); // Reset print color to white
	for(int c = 0; c < MAX_COL ; c++) // Base
		printf("---"); // Aprox. the width of columns
	printf("\n");
	for(int c = 0; c < MAX_COL ; c++) // Col Nrs
		printf(" %d ", c+1); // Cols begin at zero so add 1 for print
	printf("\n\n\x1B[0m"); // Reset print color to white again
}

// Inserts piece according to column, symbol and current player (turn)
int insertPiece(int col, int symbol, int turn){
	if( col < 0 || col >= MAX_COL ) // Error handler input column
		return -1;
	int row = getAvailableRow(col); // Stack on rows
	if(row >= 0){ // If row is available
		board[row][col] = getPieceId(symbol, turn);
		return row; // Return row placed
	}
	else return -1; // Col is full
}

// Main function with gameloop
int main() {
	// Clears the board before playing
	clearBoard();
	// Give pieces to players before playing
	givePieces();
	
	// Variable dimensions for G2 Window
	// FIRST_SPACE + (OBJECT + SPACE_TO_NEXT) * COLUMNS
	int x = SPACING+(OBJ_DIM+SPACING)*MAX_COL; 
	// FIRST_SPACE + (OBJECT + SPACE_TO_NEXT) * ROWS PLUS EXTRA 1
	int y = SPACING+(OBJ_DIM+SPACING)*(MAX_ROW+1);
	// Initialize G2 on interface, function created due to filesplit
	init_device(x, y);

	// Loop until checkWhoWins returns true
	do{
		
		viewBoard();
		viewBoard_g2(x, y);
		askTurnChoices();
	} while (!checkWhoWins(getLastRow(), getLastCol()));
	
	// View Board one last time (gameover)
	viewBoard();
	viewBoard_g2(x, y);
	
	// Workaround so G2 window doesen't close at endgame
	getchar();
	getchar();

	return 0;	
}

