#include "game.h"
#include <string>
#include <sstream>

Game::Game(int screenWidth, int screenHeight)
    : screenWidth(screenWidth)
    , screenHeight(screenHeight)
    , playerXPos(580)
    , playerYPos(640)
    , score(0)
    , paused(true)
    , acceptInput(false)
    , lastDrawn(0)
    , timeRemaining(6.0f)
    , timeBarStartWidth(400)
    , messageText("Press Enter to start!")
    , ripPos({2000, 2000})  // Inicialmente fora da tela
{
    // Inicializa a barra de tempo
    float timeBarHeight = 80;
    timeBar.x = (screenWidth / 2) - timeBarStartWidth / 2;
    timeBar.y = 980;
    timeBar.width = timeBarStartWidth;
    timeBar.height = timeBarHeight;
    timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
    
    LoadResources();
}

Game::~Game() {
    UnloadTexture(ripTexture);
    UnloadFont(gameFont);
    UnloadSound(chopSound);
    UnloadSound(deathSound);
    UnloadSound(outOfTimeSound);
}

void Game::LoadResources() {
    // Carregar texturas
    ripTexture = LoadTexture("graphics/rip.png");
    
    // Carregar fonte
    gameFont = LoadFont("fonts/KOMIKAP_.ttf");
    
    // Carregar sons
    chopSound = LoadSound("sound/chop.wav");
    deathSound = LoadSound("sound/death.wav");
    outOfTimeSound = LoadSound("sound/out_of_time.wav");
}

void Game::Init() {
    // Inicializar jogador
    player.Init(playerYPos);
    
    // Inicializar nível
    level.Reset();
    
    // Inicializar abelha
    bee.Reset();
    
    // Configurações iniciais do jogo
    score = 0;
    timeRemaining = 6.0f;
    paused = true;
    acceptInput = false;
    messageText = "Press Enter to start!";
    ripPos = {2000, 2000};  // Inicialmente fora da tela
}

void Game::Running(float deltaTime) {
    // Se o jogo está pausado, só verifica entrada para iniciar
    if (paused) {
        if (IsKeyPressed(KEY_ENTER)) {
            ResetGame();
            paused = false;
            acceptInput = true;
        }
        return;
    }
    
    // Atualizar objetos do jogo
    player.Update(deltaTime);
    bee.Update(deltaTime);
    level.Update(deltaTime);
    
    // Atualizar a barra de tempo
    UpdateTimeBar(deltaTime);
    
    // Verificar entrada do jogador
    HandleInput();
    
    // Verificar colisões
    CheckCollisions();
    
    // Controle de quando desenhar o FPS e score
    lastDrawn++;
    if (lastDrawn == 100) {
        lastDrawn = 0;
    }
    
    // Se o jogador completou sua ação, permitir nova entrada
    if (player.IsActionComplete()) {
        acceptInput = true;
    }
}

void Game::Draw() {
    // Desenhar nível (background, árvores, galhos, nuvens, tronco)
    level.Draw();
    
    // Desenhar jogador
    player.Draw();
    
    // Desenhar RIP (lápide)
    DrawTexture(ripTexture, ripPos.x, ripPos.y, WHITE);
    
    // Desenhar abelha
    bee.Draw();
    
    // Desenhar planos de fundo dos textos
    DrawRectangle(0, 30, 600, 105, ColorAlpha(BLACK, 0.6f));
    DrawRectangle(1150, 30, 1000, 105, ColorAlpha(BLACK, 0.6f));
    
    // Desenhar pontuação
    std::string scoreStr = "Score = " + std::to_string(score);
    DrawTextEx(gameFont, scoreStr.c_str(), (Vector2){20, 20}, 100, 2, WHITE);
    
    // Desenhar FPS
    std::string fpsStr = "FPS = " + std::to_string(GetFPS());
    DrawTextEx(gameFont, fpsStr.c_str(), (Vector2){1200, 20}, 100, 2, WHITE);
    
    // Desenhar barra de tempo
    DrawRectangleRec(timeBar, RED);
    
    // Desenhar mensagem quando pausado
    if (paused) {
        Vector2 textSize = MeasureTextEx(gameFont, messageText, 75, 2);
        messageTextPosition = {
            screenWidth/2.0f - textSize.x/2.0f,
            screenHeight/2.0f - textSize.y/2.0f
        };
        DrawTextEx(gameFont, messageText, messageTextPosition, 75, 2, WHITE);
    }
}

void Game::UpdateTimeBar(float deltaTime) {
    timeBar.width = timeBarWidthPerSecond * timeRemaining;
    timeRemaining -= deltaTime;
    
    // Verificar se o tempo acabou
    if (timeRemaining <= 0.0f) {
        paused = true;
        messageText = "Out of time!!";
        PlaySound(outOfTimeSound);
    }
    
    // Ativar abelha aleatoriamente se não estiver ativa
    if (!bee.IsActive()) {
        bee.Activate();
    }
}

void Game::HandleInput() {
    if (acceptInput) {
        if (IsKeyPressed(KEY_RIGHT)) {
            player.CutRight();
            score++;
            
            // Adicionar tempo
            timeRemaining += (2.0f / score) + 0.15f;
            
            // Atualizar galhos
            level.UpdateBranches(score);
            
            // Arremessar tronco para esquerda
            level.ThrowLog(Side::RIGHT);
            
            acceptInput = false;
            
            // Tocar som de corte
            PlaySound(chopSound);
        }
        
        if (IsKeyPressed(KEY_LEFT)) {
            player.CutLeft();
            score++;
            
            // Adicionar tempo
            timeRemaining += (2.0f / score) + 0.15f;
            
            // Atualizar galhos
            level.UpdateBranches(score);
            
            // Arremessar tronco para direita
            level.ThrowLog(Side::LEFT);
            
            acceptInput = false;
            
            // Tocar som de corte
            PlaySound(chopSound);
        }
    }
}

void Game::CheckCollisions() {
    // Verificar colisão com abelha
    if (bee.IsActive() && CheckCollisionRecs(bee.GetCollisionRect(), player.GetCollisionRect())) {
        // Abelha atingiu o jogador
        paused = true;
        acceptInput = false;
        
        // Posicionar a lápide com base no lado do jogador
        if (player.GetSide() == Side::LEFT) {
            ripPos = {525, 760};  // Mesma posição do código original
        } else {
            ripPos = {1200, 760};  // Mesma posição do código original
        }
        
        // Esconder jogador
        player.Hide();
        
        // Definir mensagem
        messageText = "STUNG!!";
        
        // Tocar som de morte
        PlaySound(deathSound);
    }

     // Verificar se o jogador foi esmagado por um galho
     Side branchSide = level.GetBranchSide(5); // Galho mais baixo
     Side playerSide = player.GetSide();
     
     // Debug - você pode adicionar isso temporariamente
     // TraceLog(LOG_INFO, "Branch side: %d, Player side: %d", (int)branchSide, (int)playerSide);
     
     if (branchSide != Side::NONE && branchSide == playerSide) {
         // Morte
         paused = true;
         acceptInput = false;
         
         // Posicionar a lápide com base no lado do jogador
         if (playerSide == Side::LEFT) {
             ripPos = {525, 760};
         } else {
             ripPos = {1200, 760};
         }
         
         // Esconder jogador
         player.Hide();
         
         // Alterar texto da mensagem
         messageText = "SQUISHED!!";
         
         // Tocar som de morte
         PlaySound(deathSound);
     }
}


void Game::ResetGame() {
    // Reiniciar pontuação e tempo
    score = 0;
    timeRemaining = 6.0f;
    
    // Reiniciar nível (galhos, nuvens, tronco)
    level.Reset();
    
    // Reiniciar jogador
    player.Reset(playerYPos);
    
    // Esconder lápide
    ripPos = {2000, 2000};  // Posição fora da tela
    
    // Reiniciar abelha
    bee.Reset();
    
    // Reiniciar controles
    acceptInput = true;
    paused = false;
    
    // Definir texto da mensagem
    messageText = "";
}