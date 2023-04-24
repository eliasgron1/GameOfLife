#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "raylib.h"
#include "menu.c"



#define GENERATIONS 1000
#define BOARD_ROWS 100
#define BOARD_COLS 100
#define CELL_SIZE 10
#define DELAY 100
#define ON '1'
#define OFF '0'


struct cell{
	char current;
	char future;
};


/*-------------------------------------------------------------------*
*    FUNCTION PROTOTYPES                                             *
*--------------------------------------------------------------------*/
int menu(void);
int determine_next_gen(struct cell board[BOARD_ROWS][BOARD_COLS]);
int initialize_board(struct cell board[BOARD_ROWS][BOARD_COLS]);
int read_file_to_board(struct cell board[BOARD_ROWS][BOARD_COLS],FILE *fp);
int print_current_board(struct cell board[BOARD_ROWS][BOARD_COLS]);
int randomize_file(FILE *fp);

/*********************************************************************
*    MAIN PROGRAM                                                      *
**********************************************************************/
int main(void){

FILE *fp;
struct cell board[BOARD_ROWS][BOARD_COLS] = {0,0};
int screen_width = (BOARD_COLS - 1) * CELL_SIZE;
int screen_height = (BOARD_ROWS - 1) * CELL_SIZE;
int current_generation = 0, frame_counter = 0;
int i,j;


bool restart = false;
bool start = true;


InitWindow(screen_width, screen_height, "Game Of Life");
SetTargetFPS(30);

if (menu() == true) CloseWindow();
  
Texture2D explosion = LoadTexture("resources/explosion.png");
Music music = LoadMusicStream("resources/doom.mp3");		// Load music stream and play it
PlayMusicStream(music);


initialize_board(board);

randomize_file(fp);

read_file_to_board(board,fp);



while (!WindowShouldClose()){
UpdateMusicStream(music);

if (restart==true){
    initialize_board(board);
    randomize_file(fp);
	read_file_to_board(board,fp);

	if(start == false){
		StopMusicStream(music);
		PlayMusicStream(music);
	}

	current_generation=0;
	restart = false;
}
determine_next_gen(board);



BeginDrawing();

	if (frame_counter < 272){
		DrawTexture(explosion, 300, 300, WHITE);
		DrawTexture(explosion, 10, 10, WHITE);
		restart=true,start=true, frame_counter++;
	}

	else{
	start = false;
	ClearBackground(DARKPURPLE);
	for (int i = 0; i < BOARD_ROWS ; i++) {												
		for (int j = 0; j < BOARD_COLS ; j++) {
			Rectangle rec = { i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE };
    		if (board[i][j].current == ON)												
    			DrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, RAYWHITE);
				DrawRectangleLinesEx(rec, (float)0.5, DARKGRAY);    
	}
DrawText(TextFormat("GEN %i",current_generation), 1, 1, 30, RAYWHITE);

restart = GuiButton((Rectangle){150,1,100,30}, "Restart");

	}
}
EndDrawing();




for(i=0; i < BOARD_ROWS; i++){															// update current board
	for(j=0; j < BOARD_COLS; j++){
		board[i][j].current = board[i][j].future;
  	}
}

current_generation++;

}

UnloadMusicStream(music); 
CloseAudioDevice();
CloseWindow();
return 0;
} /* END OF MAIN */




/*********************************************************************
*    FUNCTIONS                                                       *
**********************************************************************/

/*********************************************************************
 NAME: initialize_board 
 DESCRIPTION: Initialize board to all off
	Input: struct cell board[BOARD_ROWS][BOARD_COLS]
	Output: struct cell board[BOARD_ROWS][BOARD_COLS]
*********************************************************************/
int initialize_board(struct cell board[BOARD_ROWS][BOARD_COLS]){
for(int i=0; i < BOARD_ROWS; i++){		                      
	for(int j=0; j < BOARD_COLS; j++){
		board[i][j].current = OFF;
		board[i][j].future = OFF;
  	}
}
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

int determine_next_gen(struct cell board[BOARD_ROWS][BOARD_COLS]){
int neighbour_count = 0; 
int i, j;
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
return 0;
}