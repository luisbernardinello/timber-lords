#include <raylib.h>
#include "game.h"

int main() 
{
    constexpr int screenWidth = 1920;
    constexpr int screenHeight = 1080;
    
    InitWindow(screenWidth, screenHeight, "Timber!!!");
    SetTargetFPS(60);
    
    ToggleFullscreen();
    InitAudioDevice();
    
    Game game(screenWidth, screenHeight);
    
    game.Init();
    
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        
        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }
        
        game.Running(deltaTime);
        
        BeginDrawing();
            ClearBackground(RAYWHITE);
            game.Draw();
        EndDrawing();
    }
    

    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}