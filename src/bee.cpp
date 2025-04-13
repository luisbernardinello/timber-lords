#include "bee.h"
#include <cstdlib>
#include <ctime>

Bee::Bee()
    : position({2000, 800})
    , speed(0)
    , active(false)
{
    LoadTextures();
    collisionRect = {position.x, position.y, (float)beeTexture.width, (float)beeTexture.height};
}

Bee::~Bee() {
    UnloadTexture(beeTexture);
}

void Bee::LoadTextures() {
    beeTexture = LoadTexture("graphics/bee.png");
}

void Bee::Reset() {
    active = false;
    position = {2000, 800};
    collisionRect = {position.x, position.y, (float)beeTexture.width, (float)beeTexture.height};
}

void Bee::Activate() {
    if (!active) {
        // Generate random speed
        srand((int)time(0) * 10);
        speed = (rand() % 200) + 200;
        
        // Generate random height
        srand((int)time(0) * 10);
        float height = (rand() % 500) + 500;
        position = {2000, height};
        
        active = true;
    }
}

void Bee::Update(float deltaTime) {
    if (active) {
        // Move the bee
        position.x -= speed * deltaTime;
        
        // Update collision rectangle
        collisionRect.x = position.x;
        collisionRect.y = position.y;
        
        // Check if bee is off-screen
        if (position.x < -100) {
            active = false;
        }
    }
}

void Bee::Draw() const {
    if (active) {
        DrawTexture(beeTexture, position.x, position.y, WHITE);
    }
}