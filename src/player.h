#pragma once
#include <raylib.h>
#include <vector>
#include "game_types.h"

struct Animation {
    std::vector<Rectangle> frames;
    size_t currentFrame;
    float frameTime;
    float frameCounter;
    bool isPlaying;
};

class Player {
public:
    Player();
    ~Player();
    
    void Init(float height);
    void LoadTextures();
    void Update(float deltaTime);
    void Draw() const;
    void CutLeft();
    void CutRight();
    void Reset(float height);
    void Hide();
    bool IsActionComplete() const;
    
    Side GetSide() const;
    Rectangle GetCollisionRect() const;
    float GetOffsetX() const;
    
private:
    Texture2D playerTexture;
    Texture2D ripTexture;
    Vector2 playerPos;
    bool facingRight;
    bool isPlayerCutting;
    bool hidden;
    
    float playerOffsetLeft;
    float playerOffsetRight;
    
    Animation breathAnimation;
    Animation cutAnimation;
    
    void UpdateAnimation(Animation& anim, float deltaTime);
    void InitAnimations();
};