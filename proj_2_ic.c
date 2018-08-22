#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Constantes */


/** Horizontal world size. */
#define MAX_ROW 7
/** Vertical world size. */
#define MAX_COL 7

char board[MAX_ROW][MAX_COL];

void clearBoard(){
	for(int r = 0; r < MAX_ROW ; r++)
		for(int c = 0; c < MAX_COL ; c++)
			board[r][c] = 0;
}


int main(int argc, char *argv[]) {
	clearBoard();
	
	
	return 0;	
}

