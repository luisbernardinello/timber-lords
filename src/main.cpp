#include "raylib.h"
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <string>

// Function declaration
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
// Where is the player/branch? 
// Left or Right
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
    // Create a window
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "Timber!!!");
    SetTargetFPS(60);
    
    // Fullscreen mode
    ToggleFullscreen();
    
    // Initialize audio device
    InitAudioDevice();

    // Load textures
    Texture2D backgroundTexture = LoadTexture("graphics/background.png");
    Texture2D treeTexture = LoadTexture("graphics/tree.png");
    Texture2D tree2Texture = LoadTexture("graphics/tree2.png");
    Texture2D beeTexture = LoadTexture("graphics/bee.png");
    Texture2D cloudTexture = LoadTexture("graphics/cloud.png");
    Texture2D branchTexture = LoadTexture("graphics/branch.png");
    Texture2D playerTexture = LoadTexture("graphics/player.png");
    Texture2D ripTexture = LoadTexture("graphics/rip.png");
    Texture2D axeTexture = LoadTexture("graphics/axe.png");
    Texture2D logTexture = LoadTexture("graphics/log.png");

    // Load sounds
    Sound chopSound = LoadSound("sound/chop.wav");
    Sound deathSound = LoadSound("sound/death.wav");
    Sound outOfTimeSound = LoadSound("sound/out_of_time.wav");

    // Tree positions - Only 3 trees visible in your version
    Vector2 treePos = {810, 0}; // Main tree
    Vector2 tree2Pos = {20, 0};  // Left tree
    Vector2 tree3Pos = {1500, 0}; // Right tree

    // Bee setup
    Vector2 beePos = {0, 800};
    bool beeActive = false;
    float beeSpeed = 0.0f;
    // Define collision rectangle for bee
    Rectangle beeRect = {0, 0, (float)beeTexture.width, (float)beeTexture.height};

    // Cloud setup
    const int NUM_CLOUDS = 6;
    Vector2 cloudPos[NUM_CLOUDS];
    int cloudSpeeds[NUM_CLOUDS];
    bool cloudsActive[NUM_CLOUDS];

    for (int i = 0; i < NUM_CLOUDS; i++)
    {
        cloudPos[i] = {-300, i * 150.0f};
        cloudsActive[i] = false;
        cloudSpeeds[i] = 0;
    }

    // Time bar setup
    Rectangle timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.x = (screenWidth / 2) - timeBarStartWidth / 2;
    timeBar.y = 980;
    timeBar.width = timeBarStartWidth;
    timeBar.height = timeBarHeight;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    // Game state
    bool paused = true;
    int score = 0;
    
    // Message text
    const char* messageText = "Press Enter to start!";
    Vector2 messageTextPosition;
    
    // Font loading
    Font gameFont = LoadFont("fonts/KOMIKAP_.ttf");
    
    // Player setup
    Vector2 playerPos = {580, 720};
    side playerSide = side::LEFT;
    // Player collision rectangle
    Rectangle playerRect = {playerPos.x, playerPos.y, (float)playerTexture.width, (float)playerTexture.height};

    // RIP setup
    Vector2 ripPos = {600, 2000}; // Start off-screen
    
    // Axe setup
    Vector2 axePos = {2000, 830}; // Start off-screen
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Log setup
    Vector2 logPos = {810, 720};
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // Input control
    bool acceptInput = false;
    
    // Drawing control
    int lastDrawn = 0;
    
    // Clock for measuring time
    double currentTime = GetTime();
    double previousTime = currentTime;
    float deltaTime = 0.0f;

    // Initialize branch positions
    for (int i = 0; i < NUM_BRANCHES; i++) {
        branchPositions[i] = side::NONE;
    }

    // Main game loop
    while (!WindowShouldClose())
    {
        // Timing
        currentTime = GetTime();
        deltaTime = (float)(currentTime - previousTime);
        previousTime = currentTime;

        // Handle player input
        if (IsKeyPressed(KEY_ENTER))
        {
            paused = false;
            
            // Reset the time and score
            score = 0;
            timeRemaining = 6.0f;
            
            // Reset branches
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }
            
            // Hide gravestone
            ripPos.y = 2000;
            
            // Reset player position
            playerPos = {580, 720};
            playerSide = side::LEFT;
            
            // Reset bee position and state
            beeActive = false;
            beePos = {2000, 800}; // Move bee offscreen
            
            // Reset clouds positions
            for (int i = 0; i < NUM_CLOUDS; i++)
            {
                cloudsActive[i] = false;
                cloudPos[i] = {-300, i * 150.0f};
            }
            
            // Reset log position
            logActive = false;
            logPos = {810, 720};
            
            acceptInput = true;
            
            // Set the message text
            messageText = "Press Enter to start!";
        }

        if (IsKeyPressed(KEY_ESCAPE))
        {
            CloseWindow();
        }
        
        // Handle key release event
        if ((IsKeyReleased(KEY_RIGHT) || IsKeyReleased(KEY_LEFT)) && !paused)
        {
            acceptInput = true;
            
            // Hide the axe
            axePos.x = 2000;
        }
        
        // Player controls
        if (acceptInput && !paused)
        {
            if (IsKeyPressed(KEY_RIGHT))
            {
                playerSide = side::RIGHT;
                score++;
                
                // Add time
                timeRemaining += (2.0f / score) + 0.15f;
                
                // Show axe on right side
                axePos = {AXE_POSITION_RIGHT, 830};
                
                // Move player to right
                playerPos = {1200, 720};
                // Update player rectangle position
                playerRect.x = playerPos.x;
                
                // Update branches
                updateBranches(score);
                
                // Set log flying to the left
                logPos = {810, 720};
                logSpeedX = -5000;
                logActive = true;
                
                acceptInput = false;
                
                // Play chop sound
                PlaySound(chopSound);
            }
            
            if (IsKeyPressed(KEY_LEFT))
            {
                playerSide = side::LEFT;
                score++;
                
                // Add time
                timeRemaining += (2.0f / score) + 0.15f;
                
                // Show axe on left side
                axePos = {AXE_POSITION_LEFT, 830};
                
                // Move player to left
                playerPos = {580, 720};
                // Update player rectangle position
                playerRect.x = playerPos.x;
                
                // Update branches
                updateBranches(score);
                
                // Set log flying to the right
                logPos = {810, 720};
                logSpeedX = 5000;
                logActive = true;
                
                acceptInput = false;
                
                // Play chop sound
                PlaySound(chopSound);
            }
        }
        
        // Update game logic when not paused
        if (!paused)
        {
            // Update time bar
            timeBar.width = timeBarWidthPerSecond * timeRemaining;
            timeRemaining -= deltaTime;
            
            // Check if time's up
            if (timeRemaining <= 0.0f)
            {
                paused = true;
                messageText = "Out of time!!";
                PlaySound(outOfTimeSound);
            }
            
            // Setup the bee
            if (!beeActive)
            {
                // How fast is the bee
                srand((int)time(0) * 10);
                beeSpeed = (rand() % 200) + 200;
                
                // How high is the bee
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                beePos = {2000, height};
                beeActive = true;
            }
            else
            {
                // Move bee
                beePos.x -= beeSpeed * deltaTime;
                
                // Update bee collision rectangle
                beeRect.x = beePos.x;
                beeRect.y = beePos.y;
                
                // Check for collision with player
                playerRect.x = playerPos.x;
                playerRect.y = playerPos.y;
                
                if (CheckCollisionRecs(beeRect, playerRect))
                {
                    // Bee hit the player
                    paused = true;
                    acceptInput = false;
                    
                    // CORREÇÃO: Posicionar a lápide baseado no lado em que o jogador está
                    if (playerSide == side::LEFT) {
                        ripPos = {525, 760};
                    } else {
                        ripPos = {1200, 760};
                    }
                    
                    // Hide player
                    playerPos.x = 2000;
                    
                    // Set message
                    messageText = "STUNG!!";
                    
                    // Play death sound
                    PlaySound(deathSound);
                }
                
                // Has the bee reached the left edge of the screen?
                if (beePos.x < -100)
                {
                    beeActive = false;
                }
            }
            
            // Manage the clouds
            for (int i = 0; i < NUM_CLOUDS; i++)
            {
                if (!cloudsActive[i])
                {
                    // How fast is the cloud
                    srand((int)time(0) * (i + 1));
                    cloudSpeeds[i] = (rand() % 200);
                    
                    // How high is the cloud
                    srand((int)time(0) * (i + 1));
                    float height = (rand() % 150);
                    cloudPos[i] = {-200, height};
                    cloudsActive[i] = true;
                }
                else
                {
                    // Move cloud
                    cloudPos[i].x += cloudSpeeds[i] * deltaTime;
                    
                    // Has the cloud reached the right edge of the screen?
                    if (cloudPos[i].x > 1920)
                    {
                        cloudsActive[i] = false;
                    }
                }
            }
            
            // Draw the score and the frame rate once every 100 frames
            lastDrawn++;
            if (lastDrawn == 100)
            {
                lastDrawn = 0;
            }
            
            // Handle flying log
            if (logActive)
            {
                logPos.x += logSpeedX * deltaTime;
                logPos.y += logSpeedY * deltaTime;
                
                // Has the log reached the edge of the screen?
                if (logPos.x < -100 || logPos.x > 2000)
                {
                    logActive = false;
                    logPos = {810, 720};
                }
            }
            
            // Has the player been squished by a branch?
            if (branchPositions[5] == playerSide)
            {
                // Death
                paused = true;
                acceptInput = false;
                
                // CORREÇÃO: Posicionar a lápide baseado no lado em que o jogador está
                if (playerSide == side::LEFT) {
                    ripPos = {525, 760};
                } else {
                    ripPos = {1200, 760};
                }
                
                // Hide the player
                playerPos.x = 2000;
                
                // Change the text of the message
                messageText = "SQUISHED!!";
                
                // Play the death sound
                PlaySound(deathSound);
            }
        }
        
        // Drawing
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        
        // Draw background
        DrawTexture(backgroundTexture, 0, 0, WHITE);
        
        // Draw clouds
        for (int i = 0; i < NUM_CLOUDS; i++)
        {
            DrawTexture(cloudTexture, cloudPos[i].x, cloudPos[i].y, WHITE);
        }
        
        // Draw only 3 trees - one main tree and two side trees
        DrawTexture(tree2Texture, tree2Pos.x, tree2Pos.y, WHITE);  // Left tree
        DrawTexture(tree2Texture, tree3Pos.x, tree3Pos.y, WHITE);  // Right tree
        
        // Draw branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            float height = i * 150;
            
            if (branchPositions[i] == side::LEFT)
            {
                // Left side branch with 180-degree rotation
                DrawTexturePro(
                    branchTexture,
                    (Rectangle){0, 0, (float)branchTexture.width, (float)branchTexture.height},
                    (Rectangle){610, height, (float)branchTexture.width, (float)branchTexture.height},
                    (Vector2){220, 40},  // Origin point
                    180.0f,              // Rotation
                    WHITE
                );
            }
            else if (branchPositions[i] == side::RIGHT)
            {
                // Right side branch with no rotation
                DrawTexturePro(
                    branchTexture,
                    (Rectangle){0, 0, (float)branchTexture.width, (float)branchTexture.height},
                    (Rectangle){1330, height, (float)branchTexture.width, (float)branchTexture.height},
                    (Vector2){220, 40},  // Origin point
                    0.0f,                // No rotation
                    WHITE
                );
            }
            // No branch for NONE position
        }
        
        // Draw main tree
        DrawTexture(treeTexture, treePos.x, treePos.y, WHITE);
        
        // Draw player
        DrawTexture(playerTexture, playerPos.x, playerPos.y, WHITE);
        
        // Draw axe
        DrawTexture(axeTexture, axePos.x, axePos.y, WHITE);
        
        // Draw log
        DrawTexture(logTexture, logPos.x, logPos.y, WHITE);
        
        // Draw RIP
        DrawTexture(ripTexture, ripPos.x, ripPos.y, WHITE);
        
        // Draw text backgrounds - to match original
        DrawRectangle(0, 30, 600, 105, ColorAlpha(BLACK, 0.6f));
        DrawRectangle(1150, 30, 1000, 105, ColorAlpha(BLACK, 0.6f));
        
        // Draw bee
        DrawTexture(beeTexture, beePos.x, beePos.y, WHITE);
        
        // Draw score
        std::string scoreStr = "Score = " + std::to_string(score);
        DrawTextEx(gameFont, scoreStr.c_str(), (Vector2){20, 20}, 100, 2, WHITE);
        
        // Draw FPS
        std::string fpsStr = "FPS = " + std::to_string(GetFPS());
        DrawTextEx(gameFont, fpsStr.c_str(), (Vector2){1200, 20}, 100, 2, WHITE);
        
        // Draw time bar
        DrawRectangleRec(timeBar, RED);
        
        // Draw pause message properly centered
        if (paused)
        {
            Vector2 textSize = MeasureTextEx(gameFont, messageText, 75, 2);
            messageTextPosition = {
                screenWidth/2.0f - textSize.x/2.0f,
                screenHeight/2.0f - textSize.y/2.0f
            };
            DrawTextEx(gameFont, messageText, messageTextPosition, 75, 2, WHITE);
        }
        
        EndDrawing();
    }
    
    // Cleanup
    UnloadTexture(backgroundTexture);
    UnloadTexture(treeTexture);
    UnloadTexture(tree2Texture);
    UnloadTexture(beeTexture);
    UnloadTexture(cloudTexture);
    UnloadTexture(branchTexture);
    UnloadTexture(playerTexture);
    UnloadTexture(ripTexture);
    UnloadTexture(axeTexture);
    UnloadTexture(logTexture);
    UnloadFont(gameFont);
    UnloadSound(chopSound);
    UnloadSound(deathSound);
    UnloadSound(outOfTimeSound);
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}

// Function to update branches positions
void updateBranches(int seed)
{
    // Move all branches down one place
    for (int j = NUM_BRANCHES - 1; j > 0; j--) {
        branchPositions[j] = branchPositions[j - 1];
    }
    
    // Spawn a new branch at position 0
    // LEFT, RIGHT or NONE
    srand((int)time(0) + seed);
    int r = (rand() % 5);
    
    switch (r) {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    default:
        branchPositions[0] = side::NONE;
        break;
    }
}