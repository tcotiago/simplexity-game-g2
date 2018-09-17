#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <g2.h>
#include <g2_X11.h>

#include "constants.h"
#include "interface.h"
#include "game.h"

int playerTurn = 0; // What player (P1 or P2) (0 or 1)
int turnCounter = 1; // Turn Counter
int dev; // G2 Device
int row_sel, col_sel; // Column and row selected (for input)
int inventory[2][2]; // Array for possible piece types

int getLastRow(){ // Last row selected pass to checkWhoWins
	return row_sel;
}

int getLastCol(){ // Last col selected pass to checkWhoWins
	return col_sel;
}

// Initialize G2 Window
void init_device(int x, int y){
	dev=g2_open_X11(x, y); // Opens X11 window according to defines
}

// Give inventory pieces to players when game starts
void givePieces(){
	playerTurn = 0; // Resets turn counter (pieces are given at start)
	for(int p = 0; p < 2 ; p++){ // Go through both players
		inventory[p][0] = MAX_PIECE_CYLINDERS; // Give both cylinders types
		inventory[p][1] = MAX_PIECE_SQUARE; // Give both square types
	}
}

// Advances turn and changes player (P1 and P2)
int nextTurn(){ 
	playerTurn = (playerTurn + 1)%2; // 0 or 1
	return turnCounter += 1;
}

// Ends game if MAX_TURN is reached
void gameoverTie(){
	printf("No player won. Zero pieces remaining. \n");
}

// Function to print turn and inventory on each turn
void showInventory(int player){
	char* color[2] = {"White", "Red"};
	
	printf("Player %d's turn \n", (player + 1));
	printf("%s cylinders left: %d \n", color[player], inventory[player][0]);
	printf("%s cubes left: %d \n", color[player], inventory[player][1]);
}

// Function with human-machine interaction (input from user)
int askTurnChoices(){
	int piece_sel; // Piece selected
	bool nok; // Not ok, error handler
	do { 
		if(turnCounter == MAX_TURNS){ // Max turns before tie
			gameoverTie();
		}
		showInventory(playerTurn);
		do {
			printf("Turn number: %d \n", turnCounter);
			printf("Choose a valid column: \t");
			scanf("%d", &col_sel);
			nok = (col_sel < 1) || (col_sel > MAX_COL);
			col_sel--;
			if(nok)
				printf("You selected an invalid column.\n\n");
		} while (nok);
		do {
			printf("Choose a valid piece type (1 for %c or 2 for %c): ", PIECES[0], PIECES[1]);
			scanf("%d", &piece_sel);
			nok = (piece_sel < 1) || (piece_sel > 2) || (inventory[playerTurn][piece_sel - 1] <= 0); // Catch bad input
			if(nok)
				printf("You selected an invalid piece type.\n\n");	
		} while (nok);
		row_sel = insertPiece(col_sel, piece_sel -1, playerTurn);
		nok = row_sel < 0; // Full col error catcher
		if(nok)
			printf("The selected column is full.\n\n");
	} while (nok == true);
	inventory[playerTurn][piece_sel - 1]--; // Remove piece on 2D Array
	nextTurn(); // Advance 1 turn on counter
	return col_sel; // Return column where piece was placed
}

// Colors for g2_pen
enum g2_colors {
    WHITE = 0,
    BLACK = 1,
    RED = 19,
    YELLOW = 26,
};
	
// Draw circles on g2
void drawCircle(int dev, int row, int col){
	int x = SPACING + (OBJ_DIM + SPACING) * col + OBJ_DIM/2;
	int y = SPACING + (OBJ_DIM + SPACING) * (row + 1) + OBJ_DIM/2;
	g2_filled_circle(dev, x, y, OBJ_DIM/2); 
}

// Draw squares on g2
void drawSquare(int dev, int row, int col){
	int x = SPACING + (OBJ_DIM + SPACING) * col;
	int y = SPACING + (OBJ_DIM + SPACING) * (row + 1);
	g2_filled_rectangle(dev, x, y, x + OBJ_DIM, y + OBJ_DIM); 
}

// Get player/color for g2 shape fill
int getTurn_g2(int row, int col){
	// Use getTurn to know if 0 or 1 color
	int c = getTurn(row, col);
	if(c == 1)
		return  WHITE;
	return RED;
}

// Print g2-lib based Board, according to defines (constants.h)
void viewBoard_g2(int win_x, int win_y){
	int x,y;
	char* num = malloc(sizeof(char)*2); // *2 because 1 + /0, workaround char*
	num[1] = '\0'; // Second position is /0
	g2_pen(dev, BLACK); // Change color to black for backround
	g2_filled_rectangle (dev, 0, 0, win_x, win_y); // Background
	g2_pen(dev, YELLOW); // Change color to yellow for cols
	for(int c = 0; c < MAX_COL ; c++){ // Cycle to print yellow skeleton
		x = SPACING + OBJ_DIM/2 + (OBJ_DIM + SPACING)*c;
		y = win_y - SPACING;
		// Vertical columns
		g2_line (dev, x, SPACING + OBJ_DIM, x, y);
		// Column numbers
		num[0] = '1' + c;
		g2_string(dev, x - 2, OBJ_DIM - SPACING, num);
	}
	// Draw bottom divider line
	g2_line (dev, SPACING, SPACING + OBJ_DIM, win_x - SPACING,  SPACING + OBJ_DIM);
	
	for(int r = 0; r < MAX_ROW ; r++){ // From bot to top
		for(int c = 0; c < MAX_COL ; c++){  // Cols from left to right
			g2_pen(dev, getTurn_g2(r, c)); // Change color according to Player returned in getTurn_g2
			if(getSymbol(r, c) == 1) // If returned symbol is 1
				drawCircle(dev, r, c); // Draw circle
			if(getSymbol(r, c) == 2) // If returned symbol is 2
				drawSquare(dev, r, c); // Draw square
		}
	}
}
