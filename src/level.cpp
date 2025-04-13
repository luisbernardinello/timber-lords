#include "level.h"
#include <cstdlib>
#include <ctime>

Level::Level()
    : treePos({810, 0})         // Main tree position
    , tree2Pos({20, 0})         // Left tree position
    , tree3Pos({1500, 0})       // Right tree position
    , logPos({810, 720})        // Initial log position
    , logActive(false)
    , logSpeedX(0)
    , logSpeedY(0)
{
    // Initialize branches
    for (int i = 0; i < NUM_BRANCHES; i++) {
        branchPositions[i] = Side::NONE;
    }
    
    // Initialize clouds
    for (int i = 0; i < NUM_CLOUDS; i++) {
        cloudPos[i] = {-300, i * 150.0f};
        cloudsActive[i] = false;
        cloudSpeeds[i] = 0;
    }
    
    LoadTextures();
}

Level::~Level() {
    UnloadTexture(backgroundTexture);
    UnloadTexture(treeTexture);
    UnloadTexture(tree2Texture);
    UnloadTexture(branchTexture);
    UnloadTexture(cloudTexture);
    UnloadTexture(logTexture);
}

void Level::LoadTextures() {
    backgroundTexture = LoadTexture("graphics/background.png");
    treeTexture = LoadTexture("graphics/tree.png");
    tree2Texture = LoadTexture("graphics/tree2.png");
    branchTexture = LoadTexture("graphics/branch.png");
    cloudTexture = LoadTexture("graphics/cloud.png");
    logTexture = LoadTexture("graphics/log.png");
}

void Level::Reset() {
    // Reset branches
    for (int i = 0; i < NUM_BRANCHES; i++) {
        branchPositions[i] = Side::NONE;
    }
    
    // Reset log
    logActive = false;
    logPos = {810, 720};
    
    // Reset clouds
    for (int i = 0; i < NUM_CLOUDS; i++) {
        cloudsActive[i] = false;
        cloudPos[i] = {-300, i * 150.0f};
    }
}

void Level::Update(float deltaTime) {
    // Update clouds
    UpdateClouds(deltaTime);
    
    // Update log
    UpdateLog(deltaTime);
}

void Level::Draw() const {
    // Draw background
    DrawTexture(backgroundTexture, 0, 0, WHITE);
    
    // Draw clouds
    for (int i = 0; i < NUM_CLOUDS; i++) {
        if (cloudsActive[i]) {
            DrawTexture(cloudTexture, cloudPos[i].x, cloudPos[i].y, WHITE);
        }
    }
    
    // Draw side trees
    DrawTexture(tree2Texture, tree2Pos.x, tree2Pos.y, WHITE);  // Left tree
    DrawTexture(tree2Texture, tree3Pos.x, tree3Pos.y, WHITE);  // Right tree
    
    // Draw branches
    for (int i = 0; i < NUM_BRANCHES; i++) {
        float height = i * 150;
        
        if (branchPositions[i] == Side::LEFT) {
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
        else if (branchPositions[i] == Side::RIGHT) {
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
    
    // Draw log
    if (logActive) {
        DrawTexture(logTexture, logPos.x, logPos.y, WHITE);
    }
}

void Level::UpdateBranches(int seed) {
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
        branchPositions[0] = Side::LEFT;
        break;
    case 1:
        branchPositions[0] = Side::RIGHT;
        break;
    default:
        branchPositions[0] = Side::NONE;
        break;
    }
}

Side Level::GetBranchSide(int index) const {
    if (index >= 0 && index < NUM_BRANCHES) {
        return branchPositions[index];
    }
    return Side::NONE;
}

void Level::UpdateClouds(float deltaTime) {
    for (int i = 0; i < NUM_CLOUDS; i++) {
        if (!cloudsActive[i]) {
            // How fast is the cloud
            srand((int)time(0) * (i + 1));
            cloudSpeeds[i] = (rand() % 200);
            
            // How high is the cloud
            srand((int)time(0) * (i + 1));
            float height = (rand() % 150);
            cloudPos[i] = {-200, height};
            cloudsActive[i] = true;
        }
        else {
            // Move cloud
            cloudPos[i].x += cloudSpeeds[i] * deltaTime;
            
            // Has the cloud reached the right edge of the screen?
            if (cloudPos[i].x > 1920) {
                cloudsActive[i] = false;
            }
        }
    }
}

void Level::ThrowLog(Side playerSide) {
    logPos = {810, 720};
    
    if (playerSide == Side::LEFT) {
        logSpeedX = 5000;  // Move right
    } else {
        logSpeedX = -5000; // Move left
    }
    
    logSpeedY = -1500;     // Move up
    logActive = true;
}

void Level::UpdateLog(float deltaTime) {
    if (logActive) {
        logPos.x += logSpeedX * deltaTime;
        logPos.y += logSpeedY * deltaTime;
        
        // Has the log reached the edge of the screen?
        if (logPos.x < -100 || logPos.x > 2000) {
            logActive = false;
            logPos = {810, 720};
        }
    }
}