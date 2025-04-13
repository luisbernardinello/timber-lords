#pragma once
#include <raylib.h>

class Bee {
public:
    Bee();
    ~Bee();
    
    void LoadTextures();
    void Update(float deltaTime);
    void Draw() const;
    void Reset();
    void Activate();
    bool IsActive() const { return active; }
    Rectangle GetCollisionRect() const { return collisionRect; }
    
private:
    Texture2D beeTexture;
    Vector2 position;
    float speed;
    bool active;
    Rectangle collisionRect;
};