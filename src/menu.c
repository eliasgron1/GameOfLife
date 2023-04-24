#define RAYGUI_IMPLEMENTATION
#include "raygui.h"



int menu(void){
bool startgame = false;
bool exit = false;

const int screen_width = GetScreenWidth();
const int screen_height = GetScreenHeight();

InitAudioDevice();

Texture2D explosion = LoadTexture("resources/explosion.png");
Sound boom = LoadSound("resources/boom.mp3");
Music music = LoadMusicStream("resources/menu.mp3");		

GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
PlayMusicStream(music);

while((!startgame) && (!exit)){
ClearBackground(RAYWHITE);
UpdateMusicStream(music);
    BeginDrawing();
    startgame = GuiButton((Rectangle){(screen_width / 2)-75, (screen_height / 2),150,30}, "Start Game");
    
    exit = GuiButton((Rectangle){(screen_width / 2)-75, (screen_height / 2) + 40,150,30}, "Quit Game");
    
    if(WindowShouldClose()) exit = true;
    EndDrawing();
}


if(startgame==true){
BeginDrawing();

DrawTexture(explosion, 10, 10, WHITE);   
PlaySound(boom);

EndDrawing();
}


UnloadMusicStream(music); 
return exit;
}
