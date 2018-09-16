#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "validation.h"
#include "constants.h"
#include "game.h"

// Function that finds min between 2 values
int min(int value, int value2){
	if(value < value2)
		return value;
	return value2;
}
// Function that finds max between 2 values
int max(int value, int value2){
	if(value > value2)
		return value;
	return value2;
}
// Gets first X of a given column
int getFirstX(int col){
	return max(0, col - (MAX_WIN_PIECE - 1));
}
// Gets first Y of a given row
int getFirstY(int row){
	return max(0, row - (MAX_WIN_PIECE - 1));
}
// Gets last X of a given column
int getLastX(int col){
	return min(MAX_COL - MAX_WIN_PIECE, col);
}
// Gets last Y of a given row
int getLastY(int row){
	return min(MAX_ROW - MAX_WIN_PIECE, row);
}
// Function for Horizontal Validation
int horizontal_validation(int row, int col){
	int x_init = getFirstX(col);
	int x_last = getLastX(col);

	for( int x = x_init; x <= x_last; x++ ){
		int equal_color = 1;
		int equal_symbol = 1;
		int turn = getTurn(row, x);
		int symbol = getSymbol(row, x);
		if(turn > 0) // If there is a piece
			for( int i = 1 ; i < MAX_WIN_PIECE; i++ ){
				if( turn == getTurn(row, x + i) ){
					equal_color++;
				}
				if( symbol == getSymbol(row, x + i) ){
					equal_symbol++;
				}
			}
		if(equal_symbol == MAX_WIN_PIECE){
			return 2; // symbol win
		}
		if(equal_color == MAX_WIN_PIECE){
			return 1; // color win
		}
	}
	return 0; // nobody won, continue
}
// Function for Vertical Validation
int vertical_validation(int row, int col){
	int y_init = getFirstY(row);
	int y_last = getLastY(row);
	
	for( int y = y_init; y <= y_last; y++ ){
		int equal_color = 1;
		int equal_symbol = 1;
		int turn = getTurn(y, col);
		int symbol = getSymbol(y, col);
		if(turn > 0)
			for( int i = 1 ; i < MAX_WIN_PIECE; i++ ){
				if( turn == getTurn(y + i, col) )
					equal_color++;
				if( symbol == getSymbol(y + i, col) )
					equal_symbol++;
			}
		if(equal_symbol == MAX_WIN_PIECE){
			return 2; // symbol win
		}
		if(equal_color == MAX_WIN_PIECE){
			return 1; // color win
		}
	}
	return 0; // nobody won, continue
}

// Function for Diagonal validation bottom up
int diagonal_validation_bottom_up(int row, int col){
	int x_first = getFirstX(col);
	int y_first = getFirstY(row);
	
	int delta_first_x = col - x_first;
	int delta_first_y = row - y_first;
	
	int inits_first = min(delta_first_x, delta_first_y); // Number of units to increment
	
	x_first = col - inits_first;
	y_first = row - inits_first;
	
	for( int init = 0; init <= inits_first; init++ ){
		int equal_color = 1; // We have first color already
		int equal_symbol = 1; // We have first symbol already
		int x = x_first + init;
		int y = y_first + init;
		// Condition for board corners
		if ((y + MAX_WIN_PIECE > MAX_ROW)|| 
			(x + MAX_WIN_PIECE > MAX_COL))
			return 0;
		int turn = getTurn(y, x);
		int symbol = getSymbol(y, x);
		if(turn > 0)
			for( int i = 1 ; i < MAX_WIN_PIECE; i++ ){
				
				if( turn == getTurn(y + i, x + i) )
					equal_color++;
				if( symbol == getSymbol(y + i, x + i) )
					equal_symbol++;
			}
		if(equal_symbol == MAX_WIN_PIECE)
			return 2; // symbol win
		if(equal_color == MAX_WIN_PIECE)
			return 1; // color win
	}
	return 0; // nobody won, continue
}

// Function for Diagonal validation top down
int diagonal_validation_top_down(int row, int col){
	int x_first = getFirstX(col);
	int y_first = getFirstY(row);
	
	int delta_first_x = col - x_first;
	int delta_first_y = row - y_first;

	int inits_first = max(delta_first_x, delta_first_y); // Number of units to increment

	x_first = col - inits_first;
	y_first = row + inits_first;
	
	for( int init = 0; init <= inits_first; init++ ){
		int equal_color = 1; // We have first color already
		int equal_symbol = 1; // We have first symbol already
		int x = x_first + init;
		int y = y_first - init;
		// Condition for board corners
		if ((y + MAX_WIN_PIECE < MAX_ROW)|| 
			(x + MAX_WIN_PIECE > MAX_COL))
			return 0;
		int turn = getTurn(y, x);
		int symbol = getSymbol(y, x);
		if(turn > 0)
			for( int i = 1 ; i < MAX_WIN_PIECE; i++ ){
				
				if( turn == getTurn(y - i, x + i) )
					equal_color++;
				if( symbol == getSymbol(y - i, x + i) )
					equal_symbol++;
			}
		if(equal_symbol == MAX_WIN_PIECE)
			return 2; // symbol win
		if(equal_color == MAX_WIN_PIECE)
			return 1; // color win
	}
	return 0; // nobody won, continue
}

// Function to run all 4 validations
int runAllValidations(int row, int col){
	int result = 0; // 0 nobody won, 1 color win, 2 symbol win
	
	result = horizontal_validation(row, col);
	if(result > 0){
		//printf("Ganhou Horizontal %d\n", result);
		return result;
	}
	result = vertical_validation(row, col);
	if(result > 0){
		//printf("Ganhou Vertical %d\n", result);
		return result;
	}
		
	result = diagonal_validation_bottom_up(row, col);
	if(result > 0){
		//printf("Ganhou Diagonal baixo para cima %d\n", result);
		return result;
	}
		
	return diagonal_validation_top_down(row, col);
	if(result > 0){
		//printf("Ganhou Diagonal cima para baixo %d\n", result);
		return result;
	}
	return 0;
}

// Functions returns boolean to break gameLoop
bool checkWhoWins(int row, int col){
	int result = runAllValidations(row, col);
	if(result == 0){
		return false; // Nobody won
	}
	int turn = getTurn(row, col);
	int symbol = getSymbol(row, col);
	if(result == 1){ // Turn/color win
		printf("Player %d won!\n", turn);
	}
	else{ // Symbol win
		printf("Player %d won!\n", symbol);
	}
	return true; // A player has won, break loop
}
