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
/** Game pieces O and X for terminal view. */
#define PIECES "OX"
/** Game pieces dimension (G2). */
#define OBJ_DIM 30
/** Game pieces spacing (G2). */
#define SPACING 10
/** Game pieces in a sequence needed to win. */
#define MAX_WIN_PIECE 4
/** Inventory */
#define MAX_PIECE_CYLINDERS 10
#define MAX_PIECE_SQUARE 11


char board[MAX_ROW][MAX_COL]; // Char Array 2D Board
int inventory[2][2]; // Array for possible piece types
int row_sel, col_sel; // Column and row selected



void clearBoard(){
	for(int r = 0; r < MAX_ROW ; r++) // Go through all rows
		for(int c = 0; c < MAX_COL ; c++) // Go through all cols
			board[r][c] = 0; // Reset all rows and cols
	//playerTurn = 1;
	for(int p = 0; p < 2 ; p++){ // Go through all players
		inventory[p][0] = MAX_PIECE_CYLINDERS;
		inventory[p][1] = MAX_PIECE_SQUARE;
	}
		
			
}

char* getPrintableSymbol(char c){
	if(c == 4) return "\x1B[97m0\x1B[0m"; // White Cylinder
	if(c == 5) return "\x1B[31m0\x1B[0m"; // Red Cylinder
	if(c == 6) return "\x1B[97mX\x1B[0m"; // White Square
	if(c == 7) return "\x1B[31mX\x1B[0m"; // Red Square
	
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
	// 4 + 0-1 * 2 + color/turn
	
	// 4 White O 
	// 5 Red O
	// 6 White X
	// 7 Red X
}

char getPiece (int row, int col){ // Asks for cell on Array
	return board[row][col]; // Returns a character from the 2D Array
}

int getTurn (int row, int col){ // Get color
	char piece = getPiece(row, col);
	if(piece==0)
		return 0;
	return piece%2 + 1;
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

int insertPiece(int col, int symbol, int turn){
	if( col < 0 || col >= MAX_COL ) // Error handler input column
		return -1;
	int row = getAvailableRow(col); // Stack on rows
	if(row >= 0){ // If row is available
		board[row][col] = getPieceId(symbol, turn);
		return row;
	}
	else return -1; // Col is full
}

///////////////// NEW FILE (HMI's)
int playerTurn = 0;
int turnCounter = 1;

int nextTurn(){ // Advances turn and changes player (P1 and P2)
	playerTurn = (playerTurn + 1)%2; // 0 or 1
	return turnCounter += 1; // Advances Turn Counter on player change
}

void showInventory(int player){
	char* color[2] = {"White", "Red"};
	
	printf("Player %d's turn \n", (player + 1));
	printf("%s cylinders left: %d \n", color[player], inventory[player][0]);
	printf("%s cubes left: %d \n", color[player], inventory[player][1]);

}

int askTurnChoices(){

	int piece_sel; // Piece selected
	bool nok; // Not ok, error handler
	
	do { 
		if(turnCounter == 43){ // Max turns before tie
			clearBoard();
			//gameOverTie();
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
	} while (nok);
	
	inventory[playerTurn][piece_sel - 1]--;
	nextTurn(); // Advance 1 turn
	return col_sel;
}
	///////////////// HMI End
	
	///////////////// G2 Start

enum g2_colors {
    WHITE = 0,
    BLACK = 1,
    RED = 19,
    YELLOW = 26,
};
	
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

int getTurn_g2(int row, int col){
	int c = getTurn(row, col);
	if(c == 1)
		return  WHITE;
	return RED;
	
}

void viewBoard_g2(int dev, int win_x, int win_y){
	int x,y;
	char* num = malloc(sizeof(char)*2); // *2 para ser 1 + /0
	num[1] = '\0';
	g2_pen(dev, BLACK);
	g2_filled_rectangle (dev, 0, 0, win_x, win_y); // Background
	g2_pen(dev, YELLOW);
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
			g2_pen(dev, getTurn_g2(r, c));
			if(getSymbol(r, c) == 1)
				drawCircle(dev, r, c);
			if(getSymbol(r, c) == 2)
				drawSquare(dev, r, c);
		}
	}
}
///////////////// G2 (end)

///////////////// WinChecker - start
int min(int value, int value2){
	if(value < value2)
		return value;
	return value2;
}

int max(int value, int value2){
	if(value > value2)
		return value;
	return value2;
}

int getFirstX(int col){
	return max(0, col - (MAX_WIN_PIECE - 1));
}

int getFirstY(int row){
	return max(0, row - (MAX_WIN_PIECE - 1));
}

int getLastX(int col){
	return min(MAX_COL - MAX_WIN_PIECE, col);
}

int getLastY(int row){
	return min(MAX_ROW - MAX_WIN_PIECE, row);
}

int horizontal_validation(int row, int col){
	//printf("Horizontal %d\n", row);
	int x_init = getFirstX(col);
	int x_last = getLastX(col);

	for( int x = x_init; x <= x_last; x++ ){
		int equal_color = 1; // já temos a primeira cor igual
		int equal_symbol = 1;
		//printf("%d %d %d \n", x_init, x, x_last);
		int turn = getTurn(row, x);
		int symbol = getSymbol(row, x);
		if(turn > 0)
			for( int i = 1 ; i < MAX_WIN_PIECE; i++ ){
				
				if( turn == getTurn(row, x + i) )
					equal_color++;
				if( symbol == getSymbol(row, x + i) )
					equal_symbol++;
				//printf("%d %d %d - %d %d \n", x + i, getTurn(row, x + i), getSymbol(row, x + i), equal_color, equal_symbol);
			}
		if(equal_symbol == MAX_WIN_PIECE)
			return 2; // symbol win
		if(equal_color == MAX_WIN_PIECE)
			return 1; // color win
	}
	return 0; // nobody won, continue
}

int vertical_validation(int row, int col){
	//printf("Vertical %d\n", col);
	int y_init = getFirstY(row);
	int y_last = getLastY(row);
	for( int y = y_init; y <= y_last; y++ ){
		int equal_color = 1; // já temos a primeira cor
		int equal_symbol = 1; // já temos o primeiro simbolo
		//printf("%d %d %d \n", x_init, x, x_last); // DEBUG
		int turn = getTurn(y, col);
		int symbol = getSymbol(y, col);
		if(turn > 0)
			for( int i = 1 ; i < MAX_WIN_PIECE; i++ ){
				
				if( turn == getTurn(y + i, col) )
					equal_color++;
				if( symbol == getSymbol(y + i, col) )
					equal_symbol++;
				//printf("%d %d %d - %d %d \n", x + i, getTurn(row, x + i), getSymbol(row, x + i), equal_color, equal_symbol);
			}
		if(equal_symbol == MAX_WIN_PIECE)
			return 2; // symbol win
		if(equal_color == MAX_WIN_PIECE)
			return 1; // color win
	}
	return 0; // nobody won, continue
}


int diagonal_validation_bottom_up(int row, int col){
	int x_first = getFirstX(col);
	int y_first = getFirstY(row);
	
		
	int delta_first_x = col - x_first;
	int delta_first_y = row - y_first;
	
	//printf("x_last=%d, x_first=%d, y_last=%d, y_first=%d, \n", x_last, x_first, y_last, y_first);
	//printf("delta_first_x=%d, delta_first_y=%d, delta_last_x=%d, delta_last_y=%d \n", delta_first_x, delta_first_y, delta_last_x, delta_last_y);
	int inits_first = min(delta_first_x, delta_first_y); // Numero incrementos
	//printf("row=%d, col=%d, x_first=%d, y_first=%d, inits_first=%d, inits_last=%d", row, col, x_first, y_first, inits_first, inits_last);
	x_first = col - inits_first;
	y_first = row - inits_first;
	//printf("row=%d, col=%d, x_first=%d, y_first=%d, inits_first=%d, inits_last=%d \n", row, col, x_first, y_first, inits_first, inits_last);
	
	
	for( int init = 0; init <= inits_first; init++ ){
		int equal_color = 1; // já temos a primeira cor igual
		int equal_symbol = 1;
		int x = x_first + init;
		int y = y_first + init;
		// Condição para cantos board
		if ((y + MAX_WIN_PIECE > MAX_ROW)|| 
			(x + MAX_WIN_PIECE > MAX_COL))
			return 0;
		//printf("%d %d %d \n", x_init, x, x_last);
		int turn = getTurn(y, x);
		int symbol = getSymbol(y, x);
		//printf("%d|%d  turn = %d, symbol = %d, x = %d, y = %d\n", init, inits_first, turn, symbol, x, y);
		if(turn > 0)
			for( int i = 1 ; i < MAX_WIN_PIECE; i++ ){
				
				if( turn == getTurn(y + i, x + i) )
					equal_color++;
				if( symbol == getSymbol(y + i, x + i) )
					equal_symbol++;
				//printf("%d %d %d - %d %d \n", x + i, getTurn(row, x + i), getSymbol(row, x + i), equal_color, equal_symbol);
			}
		if(equal_symbol == MAX_WIN_PIECE)
			return 2; // symbol win
		if(equal_color == MAX_WIN_PIECE)
			return 1; // color win
	}
	return 0; // nobody won, continue
}

int diagonal_validation_top_down(int row, int col){
	int x_first = getFirstX(col);
	int x_last = getLastX(col);
	int y_first = getFirstY(row);
	int y_last = getLastY(row);
	int delta_first_x = col - x_first;
	int delta_first_y = row - y_first;

	int delta_last_x = x_last - x_first;
	int delta_last_y = y_last - y_first;
	//printf("y_last=%d, y_first=%d\n", y_last, y_first);
	int inits_first = max(delta_first_x, delta_last_y); // Numero incrementos
	int inits_last = max(delta_last_x, delta_first_y); // Bug
	
	x_first = col - inits_first;
	y_first = row + inits_first;
	//printf("row=%d, col=%d, x_first=%d, y_first=%d, inits_first=%d, inits_last=%d, delta_last_x=%d, delta_last_y=%d\n", row, col, x_first, y_first, inits_first, inits_last, delta_last_x, delta_last_y);
	for( int init = 0; init <= inits_last; init++ ){
		int equal_color = 1; // já temos a primeira cor igual
		int equal_symbol = 1;
		int x = x_first + init;
		int y = y_first - init;
		//printf("%d %d %d \n", x_init, x, x_last);
		int turn = getTurn(y, x);
		int symbol = getSymbol(y, x);
		//printf("   %d|%d  turn = %d, symbol = %d, x = %d, y = %d\n", init, inits_last, turn, symbol, x, y);
		if(turn > 0)
			for( int i = 1 ; i < MAX_WIN_PIECE; i++ ){
				
				if( turn == getTurn(y - i, x + i) )
					equal_color++;
				if( symbol == getSymbol(y - i, x + i) )
					equal_symbol++;
				//printf("%d %d %d - %d %d \n", x + i, getTurn(row, x + i), getSymbol(row, x + i), equal_color, equal_symbol);
			}
		if(equal_symbol == MAX_WIN_PIECE)
			return 2; // symbol win
		if(equal_color == MAX_WIN_PIECE)
			return 1; // color win
	}
	return 0; // nobody won, continue
}

int runAllValidations(int row, int col){
	int result = 0;
	
	result = horizontal_validation(row, col);
	if(result > 0){
		printf("Ganhou Horizontal %d\n", result);
		return result;
	}
	result = vertical_validation(row, col);
	if(result > 0){
		printf("Ganhou Vertical %d\n", result);
		return result;
	}
		
	result = diagonal_validation_bottom_up(row, col);
	if(result > 0){
		printf("Ganhou Diag bot up %d\n", result);
		return result;
	}
		
	return diagonal_validation_top_down(row, col);
	if(result > 0){
		printf("Ganhou Diag top down %d\n", result);
		return result;
	}
	return 0;
}

bool checkWhoWins(int row, int col){
	int result = runAllValidations(row, col);
	if(result == 0) // Nobody won
		return false; 
		
	int turn = getTurn(row, col);
	int symbol = getSymbol(row, col);
	if(result == 1) // Have a winner
		printf("Player %d won!\n", turn);
	else 
		printf("Player %d won!\n", symbol);
	return true;
}




///////////////// (wincheck end)


int main() {

	// Clears the board before playing
	clearBoard();
	
	// Variable dimensions
	// FIRST_SPACE + (OBJECT + SPACE_TO_NEXT) * COLUMNS
	int x = SPACING+(OBJ_DIM+SPACING)*MAX_COL; 
	// FIRST_SPACE + (OBJECT + SPACE_TO_NEXT) * ROWS PLUS EXTRA 1
	int y = SPACING+(OBJ_DIM+SPACING)*(MAX_ROW+1);
	int dev=g2_open_X11(x, y); // Opens X11 window according to defines
	
	// Debugging manual insert
	//insertPiece(0, 0, 0);
	//insertPiece(1, 1, 1);
	//insertPiece(2, 0, 0);
	//insertPiece(3, 1, 1);

	//insertPiece(4, 1, 0);
	//insertPiece(4, 0, 0);
	//insertPiece(4, 0, 0);
	//insertPiece(4, 1, 1);
	
	//insertPiece(4, 1, 1);
	//insertPiece(4, 1, 1);
	//insertPiece(4, 1, 1);
	//insertPiece(2, 1, 1);
	//insertPiece(3, 0, 0);
	//insertPiece(3, 0, 0);
	
	//insertPiece(5, 0, 0);
	//insertPiece(2, 0, 0);
	//insertPiece(2, 0, 0);
	
	//insertPiece(0, 0, 0);
	//insertPiece(1, 0, 1);
	//insertPiece(1, 1, 0);
	//insertPiece(2, 1, 0);
	//insertPiece(2, 1, 0);
	//insertPiece(2, 1, 0);
	//insertPiece(3, 1, 0);
	//insertPiece(3, 1, 0);
	//insertPiece(3, 0, 1);
	
	//printf("%d Horizontal win: \n", horizontal_validation(0, 1));	
	//printf("%d Vertical win:  \n", vertical_validation(0, 3));
	//printf("%d Diag win:  \n", diagonal_validation_bottom_up(0, 0));

	do{
		
		viewBoard();
		viewBoard_g2(dev, x, y);
		askTurnChoices();
	} while (!checkWhoWins(row_sel, col_sel));
	
	viewBoard(); // Last render after winning game Ascii & G2
	viewBoard_g2(dev, x, y);

	getchar();
	getchar();
	
	return 0;	
}

