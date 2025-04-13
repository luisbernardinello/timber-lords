#pragma once
#include <raylib.h>
#include "player.h"
#include "bee.h"
#include "level.h"
#include <string>
#include <ctime>

class Game {
    public:
        Game(int screenWidth, int screenHeight);
        ~Game();
        void Running(float deltaTime);
        void Draw();
        void Init();
        
    private:
        // Constantes
        const int screenWidth;
        const int screenHeight;
        const float playerXPos;
        const float playerYPos;
        
        // Objetos do jogo
        Player player;
        Bee bee;
        Level level;
        
        // Texturas e fontes
        Texture2D ripTexture;
        Font gameFont;
        
        // Sons
        Sound chopSound;
        Sound deathSound;
        Sound outOfTimeSound;
        
        // Estado do jogo
        int score;
        bool paused;
        bool acceptInput;
        int lastDrawn;
        
        // Barra de tempo
        Rectangle timeBar;
        float timeRemaining;
        float timeBarWidthPerSecond;
        float timeBarStartWidth;
        
        // Mensagens
        const char* messageText;
        Vector2 messageTextPosition;
        
        // Lápide (RIP)
        Vector2 ripPos;
        
        void LoadResources();
        void UpdateTimeBar(float deltaTime);
        void HandleInput();
        void CheckCollisions();
        void ResetGame();
};