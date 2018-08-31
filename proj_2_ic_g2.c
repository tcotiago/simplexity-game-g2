#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <g2.h>
#include <g2_X11.h>

/* Constants */
/** Horizontal board size. */
#define MAX_ROW 7
/** Vertical board size. */
#define MAX_COL 7
/** Game pieces O and X */
#define PIECES "OX"

#define OBJ_DIM 30
#define SPACING 10

enum g2_colors {
    WHITE = 0,
    BLACK = 1,
    GREY = 26, // yellow
    BLUE = 3,
    RED = 19,
};

//#define
//#define
//#define
//#define
//#define

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
	else return (piece>>1) - (sizeof(PIECES)-1) + 1;
}
	
void viewBoard(){
	
	
	
	for(int r = MAX_ROW -1; r >= 0 ; r--){ // From top to bottom
		for(int c = 0; c < MAX_COL ; c++){  // Cols from left to right
			printf(" %s ", getPrintableSymbol(board[r][c])); // Print symbol as string
			
		}
		printf("\n");
	}
	printf("\x1B[90m"); // Reset print color to white
	for(int c = 0; c < MAX_COL ; c++) // Base
		printf("---");
	printf("\n");
	for(int c = 0; c < MAX_COL ; c++) // Col Nrs
		printf(" %d ", c+1);
	printf("\n\n\x1B[0m"); // Reset print color to white
	
	
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

	///////////////// G2
void drawCircle(int dev, int row, int col){
	int x = SPACING + (OBJ_DIM + SPACING) * col + OBJ_DIM/2;
	int y = SPACING + (OBJ_DIM + SPACING) * (row + 1) + OBJ_DIM/2;
	g2_filled_circle(dev, x, y, OBJ_DIM/2); 
	//printf("X %d, Y %d, R %d\n", x, y, OBJ_DIM/2);
}

void drawSquare(int dev, int row, int col){
	int x = SPACING + (OBJ_DIM + SPACING) * col;
	int y = SPACING + (OBJ_DIM + SPACING) * (row + 1);
	g2_filled_rectangle(dev, x, y, x + OBJ_DIM, y + OBJ_DIM); 
	//printf("X %d, Y %d, R %d\n", x, y, OBJ_DIM/2);
}

int getColor_g2(int row, int col){
	int c = getColor(row, col);
	if(c == 0)
		return  WHITE;
	return RED;
	
}

void viewBoard_g2(int dev, int win_x, int win_y){
	int x,y;
	char* num = malloc(sizeof(char)*2);
	num[1] = '\0';
	g2_pen(dev, BLACK);
	g2_filled_rectangle (dev, 0, 0, win_x, win_y);
	g2_pen(dev, GREY);
	for(int c = 0; c < MAX_COL ; c++){
		x = SPACING + OBJ_DIM/2 + (OBJ_DIM + SPACING)*c;
		y = win_y - SPACING;
		g2_line (dev, x, SPACING + OBJ_DIM, x, y);
		num[0] = '1' + c;
		g2_string(dev, x - 2, OBJ_DIM - SPACING, num);
	}
	g2_line (dev, SPACING, SPACING + OBJ_DIM, win_x - SPACING,  SPACING + OBJ_DIM);
	
	for(int r = 0; r < MAX_ROW ; r++){ // From bot to top
		for(int c = 0; c < MAX_COL ; c++){  // Cols from left to right
			g2_pen(dev, getColor_g2(r, c));
			if(getSymbol(r, c) == 1)
				drawCircle(dev, r, c);
			if(getSymbol(r, c) == 2)
				drawSquare(dev, r, c);
			//printf("--%d--",getSymbol(r,c));
		}
	}
	//g2_line (int dev, double x1, double y1, double x2, double y2)
	//g2_filled_circle(dev, r, c, 10); 
	//g2_filled_rectangle (int dev, double x1, double y1, double x2, double y2)
	//g2_string (int dev, double x, double y, const char *text)
	
}
	/////////////////


int main() {
	//printf("%d\n", sizeof(PIECES));
	clearBoard();
	int x = SPACING+(OBJ_DIM+SPACING)*MAX_COL;
	int y = SPACING+(OBJ_DIM+SPACING)*(MAX_ROW+1);
	int dev=g2_open_X11(x, y);
	// Debugging manual insert
	insertPiece(0, 0, 0);
	insertPiece(0, 1, 1);
	insertPiece(0, 0, 1);
	insertPiece(0, 1, 0);
	insertPiece(0, 1, 0);	
	viewBoard(); // Remover ao organizar interaccao
	viewBoard_g2(dev, x, y);
	//////////////////////////
	
	askTurnChoices();
	viewBoard_g2(dev, x, y);
	askTurnChoices();
	viewBoard_g2(dev, x, y);
	askTurnChoices();
	viewBoard_g2(dev, x, y);
	askTurnChoices();
	viewBoard_g2(dev, x, y);
	
	getchar();
	getchar();
	//printf("Simbolo %c, cor %d, \n", PIECES[getSymbol(0, 0)-1], getColor(0, 0));
	//printf("Simbolo %c, cor %d, \n", PIECES[getSymbol(1, 0)-1], getColor(1, 0));
	//printf("Simbolo %c, cor %d, \n", PIECES[getSymbol(2, 0)-1], getColor(2, 0));
	//board[0][0] = 4;
	//board[6][6] = 7;
	
	
	return 0;	
}

