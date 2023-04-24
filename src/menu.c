#define RAYGUI_IMPLEMENTATION
#include "raygui.h"



int menu(void){
InitAudioDevice();

Texture2D star = LoadTexture("resources/star.png");
Texture2D start = LoadTexture("resources/start.png");
Texture2D heart = LoadTexture("resources/heart.png");
Texture2D pingu = LoadTexture("resources/pingu.png");

Sound boom = LoadSound("resources/boom.mp3");
Music music = LoadMusicStream("resources/pou.mp3");		

bool startgame = false;
bool exit = false;

const int screen_width = GetScreenWidth();
const int screen_height = GetScreenHeight();

int rotation = 0;
int frameWidth = pingu.width;
int frameHeight = pingu.height;


GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
PlayMusicStream(music);

Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
Rectangle destRec = { (screen_width / 2) - 300, (screen_height / 2) - 200, frameWidth*2.0f, frameHeight*2.0f };

Vector2 origin = { (float)frameWidth, (float)frameHeight };
Vector2 position_start = { (float)(screen_width / 2) + 150, (float)(screen_height / 2) - 300 };
Vector2 position_heart = { (float)(screen_width / 2) - 450, (float)(screen_height / 2) + 250 };

while((!startgame) && (!exit)){
ClearBackground(YELLOW);
UpdateMusicStream(music);

    rotation++;

    BeginDrawing();
    startgame = GuiButton((Rectangle){(screen_width / 2)-75, (screen_height / 2),150,30}, "Start Game");
    exit = GuiButton((Rectangle){(screen_width / 2)-75, (screen_height / 2) + 40,150,30}, "Quit Game");
    
    DrawTexture(star, (screen_width / 2)  + 50, (screen_height / 2) + 200, WHITE);
    DrawTexturePro(pingu, sourceRec, destRec, origin, (float)rotation, WHITE);
    DrawTextureEx(start, position_start, 30, 1, WHITE);
    DrawTextureEx(heart, position_heart, -30, 1, WHITE);

    if(WindowShouldClose()) exit = true;
    EndDrawing();
}


if(startgame==true){
PlaySound(boom);
}


UnloadTexture(star);
UnloadTexture(start);
UnloadTexture(heart);
UnloadTexture(pingu);
UnloadMusicStream(music); 
return exit;
}
