#pragma once
#include <raylib.h>
#include <vector>
#include "game_types.h"

class Level {
public:
    Level();
    ~Level();
    
    void LoadTextures();
    void Update(float deltaTime);
    void Draw() const;
    void Reset();
    void UpdateBranches(int seed);
    
    // Getters
    Side GetBranchSide(int index) const;
    
    // Cloud management
    void UpdateClouds(float deltaTime);
    
    // Log management
    void ThrowLog(Side playerSide);
    void UpdateLog(float deltaTime);
    
    static const int NUM_BRANCHES = 6;
    
private:
    // Textures
    Texture2D backgroundTexture;
    Texture2D treeTexture;
    Texture2D tree2Texture;
    Texture2D branchTexture;
    Texture2D cloudTexture;
    Texture2D logTexture;
    
    // Branch positions
    Side branchPositions[NUM_BRANCHES];
    
    // Tree positions
    Vector2 treePos;      // Main tree
    Vector2 tree2Pos;     // Left tree
    Vector2 tree3Pos;     // Right tree
    
    // Cloud setup
    static const int NUM_CLOUDS = 6;
    Vector2 cloudPos[NUM_CLOUDS];
    int cloudSpeeds[NUM_CLOUDS];
    bool cloudsActive[NUM_CLOUDS];
    
    // Log setup
    Vector2 logPos;
    bool logActive;
    float logSpeedX;
    float logSpeedY;
};