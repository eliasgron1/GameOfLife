/***************************************************************************
 *   Copyright (C) $Year$ by $Author$   *
 *   $Email$   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/*********************************************************************

 1.  NAME
	   GameOfLife.c

 2.  DESCRIPTION
	   Conway's Game of Life

 3.  VERSIONS
       Original:
         12.4.2023 / Elias Grönholm


**********************************************************************/

/*-------------------------------------------------------------------*
*    HEADER FILES                                                    *
*--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>

/*-------------------------------------------------------------------*
*    GLOBAL VARIABLES AND CONSTANTS                                  *
*--------------------------------------------------------------------*/
/* Global constants */
#define GENERATIONS 10000  
#define BOARD_ROWS 25
#define BOARD_COLS 50
#define ON '0'
#define OFF ' '

/* Global structures */
struct cell{
	char current;
	char future;
};


/*-------------------------------------------------------------------*
*    FUNCTION PROTOTYPES                                             *
*--------------------------------------------------------------------*/
int initialize_file(FILE *fp);															
int randomize_file(FILE *fp);	    													
int read_file_to_board(struct cell board[BOARD_ROWS][BOARD_COLS],FILE *fp);					
int print_current_board(struct cell board[BOARD_ROWS][BOARD_COLS],int current_generation);	
int update_board(struct cell board[BOARD_ROWS][BOARD_COLS]);					    	    
int update_file(struct cell board[BOARD_ROWS][BOARD_COLS],FILE *fp, int current_generation);						
int ask_for_user_input(int *ptr_input);																										




/*********************************************************************
*    MAIN PROGRAM                                                      *
**********************************************************************/
int main(void){
FILE *fp;
struct cell board[BOARD_ROWS][BOARD_COLS] = {0,0};	
int current_generation = 0, neighbour_count = 0, delay = 0;
int *ptr_input = &delay;
int i,j;


initscr();


initialize_file(fp);																		// initialize file to all OFF then randomize
randomize_file(fp);

ask_for_user_input(ptr_input);


if(has_colors() == FALSE){
    printf("Your terminal does not support color\n");
    endwin();
	refresh();
    exit(1);
}

start_color();
init_pair(1, COLOR_WHITE, COLOR_BLUE);
bkgdset(COLOR_PAIR(1));



while (current_generation < GENERATIONS){
read_file_to_board(board,fp);																// read file to current board

print_current_board(board,current_generation);												// print current board to screen



for(i=1; i < BOARD_ROWS-1; i++){
	for(j=1; j < BOARD_COLS-1; j++){
		if(board[i-1][j-1].current == ON){ // 	 a) i-1 j-1
			neighbour_count++;
		}
		if(board[i][j-1].current   == ON){ //    b) i j-1
			neighbour_count++;
		}
		if(board[i+1][j-1].current == ON){ //    c) i+1 j-1 
			neighbour_count++;
		}
		if(board[i-1][j].current   == ON){ //    d) i-1 j 
			neighbour_count++;
		}
		if(board[i+1][j].current   == ON){ //    f) i+1 j 
			neighbour_count++;
		}
		if(board[i-1][j+1].current == ON){ //    g) i-1 j+1 
			neighbour_count++;
		}
		if(board[i][j+1].current   == ON){ //    h) i j+1 
			neighbour_count++;
		}
		if(board[i+1][j+1].current == ON){ //    i) i+1 j+1
			neighbour_count++;
		}

		if( (neighbour_count <= 1) || (neighbour_count >= 4) ){
			board[i][j].future = OFF;
		}
		else if( (neighbour_count == 2 || neighbour_count == 3) && (board[i][j].current == ON) ){
			board[i][j].future = ON;
		}
		else if( (neighbour_count == 3) && (board[i][j].current == OFF) ){
			board[i][j].future = ON;
		}
		neighbour_count = 0;
	}
}

update_board(board);																		// update current board to what future board was

update_file(board,fp,current_generation);													// update file to what current board is	

napms(delay);
current_generation++;

}

attroff(COLOR_PAIR(1));
getch();
endwin();
return 0;
} 
/* end of main */




/*********************************************************************
*    FUNCTIONS                                                       *
**********************************************************************/

/*********************************************************************
 NAME: initialize_file
 DESCRIPTION: Initializes file to all OFF
	Input: int *ptr_input
	Output: int *ptr_input
*********************************************************************/
int initialize_file(FILE *fp){
fp = fopen("board.txt", "w");
for(int i=0; i < BOARD_ROWS; i++){		                      
	for(int j=0; j < BOARD_COLS; j++){
		fprintf(fp,"%c", OFF);													
  	}
	fprintf(fp, "\n");
}
fclose(fp);
return 0;
}


/*********************************************************************
 NAME: randomize_file
 DESCRIPTION: randomizes file
	Input: FILE *fp
	Output: FILE *fp
*********************************************************************/
int randomize_file(FILE *fp){
srand(time(NULL));
fp = fopen("board.txt", "w");

for(int i=0; i < BOARD_ROWS; i++){		                      
	for(int j=0; j < BOARD_COLS; j++){
		if(rand() % 2 == 0){
			fprintf(fp,"%c", ON);
		}
		else{
			fprintf(fp,"%c", OFF);
		}
  	}
	fprintf(fp, "\n");
}
fclose(fp);
return 0;
}



/*********************************************************************
 NAME: read_file_to_board
 DESCRIPTION: read file to current board
	Input: struct cell board[BOARD_ROWS][BOARD_COLS], FILE *fp
	Output: struct cell board[BOARD_ROWS][BOARD_COLS], FILE *fp
*********************************************************************/
int read_file_to_board(struct cell board[BOARD_ROWS][BOARD_COLS],FILE *fp){				
char state, lineend; 
int column,row;
												
fp = fopen("board.txt", "r");											
if(fp == NULL){
	printf("Error opening file");			
	return 1;
}
for(row=0; row < BOARD_ROWS; row++){
	for(column=0; column < BOARD_COLS; column++){
		fscanf(fp, "%c", &state);
		board[row][column].current = state;
	}
	fscanf(fp, "%c", &lineend);
}
fclose(fp);
return 0;
}




/*********************************************************************
 NAME: update_board
 DESCRIPTION: update current board to what future board was
	Input: struct cell board[BOARD_ROWS][BOARD_COLS]
	Output: struct cell board[BOARD_ROWS][BOARD_COLS]
*********************************************************************/
int update_board(struct cell board[BOARD_ROWS][BOARD_COLS]){	
for(int i=0; i < BOARD_ROWS; i++){								
	for(int j=0; j < BOARD_COLS; j++){
		board[i][j].current = board[i][j].future;
  	}
}
return 0;
}


/*********************************************************************
 NAME: update_file 
 DESCRIPTION: update file to what current board is
	Input: struct cell board[BOARD_ROWS][BOARD_COLS], FILE *fp, int current_generation
	Output: struct cell board[BOARD_ROWS][BOARD_COLS], FILE *fp, int current_generation
*********************************************************************/
int update_file(struct cell board[BOARD_ROWS][BOARD_COLS], FILE *fp, int current_generation){
fp = fopen("board.txt", "w");
											
for(int i=0; i < BOARD_ROWS; i++){								
	for(int j=0; j < BOARD_COLS; j++){
		fprintf(fp,"%c", board[i][j].future);
  	}
	fprintf(fp, "\n");
}
fclose(fp);
return 0;
}

/*********************************************************************
 NAME: ask_for_user_input
 DESCRIPTION: Asks user for input
	Input: int *ptr_input
	Output: int *ptr_input
*********************************************************************/

int ask_for_user_input(int *ptr_input){
int speed = 0, check = 0;
printw("Enter speed 0-100: ");
check = scanw("%d", &speed);
refresh();
if((speed>0 && speed<=100) && check == 1){
	speed = ((speed * (-1)) + 150);
	*ptr_input = speed;
}
else{
	erase();
	printw("Invalid speed, try again\n");
	ask_for_user_input(ptr_input);
	refresh();
}
return 0;
}


/*********************************************************************
 NAME: print_current_board
 DESCRIPTION: print current board to screen
	Input: struct cell board[BOARD_ROWS][BOARD_COLS], int current_generation
	Output: struct cell board[BOARD_ROWS][BOARD_COLS], int current_generation
*********************************************************************/
int print_current_board(struct cell board[BOARD_ROWS][BOARD_COLS], int current_generation){
erase();
printw("GEN: %d\n", current_generation);
for(int i=0; i < BOARD_ROWS; i++){								// print current board
	for(int j=0; j < BOARD_COLS; j++){
		printw("%c ", board[i][j].current);						
  	}
	printw("\n");
	printw("\r");
}

refresh();
return 0;
}
