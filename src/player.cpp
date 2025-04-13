#include "player.h"

Player::Player() 
    : playerPos({0, 0})
    , facingRight(false)
    , isPlayerCutting(false)
    , hidden(false)
    , playerOffsetLeft(520.0f)
    , playerOffsetRight(1000.0f)
    
{
    LoadTextures();
    InitAnimations();
}

Player::~Player() {
    UnloadTexture(playerTexture);
    UnloadTexture(ripTexture);
}

void Player::LoadTextures() {
    playerTexture = LoadTexture("graphics/man.png");
    ripTexture = LoadTexture("graphics/rip.png");
}

void Player::InitAnimations() {
    // Setup breath animation (frames 2 and 3)
    breathAnimation.frames.clear();
    breathAnimation.frames.push_back((Rectangle){ 2*527, 0, 527, 413 });  // Frame 2
    breathAnimation.frames.push_back((Rectangle){ 3*527, 0, 527, 413 });  // Frame 3
    breathAnimation.currentFrame = 0;
    breathAnimation.frameTime = 0.35f;  // 350ms between frames
    breathAnimation.frameCounter = 0.0f;
    breathAnimation.isPlaying = true;
    
    // Setup cut animation (frames 0, 1, 0)
    cutAnimation.frames.clear();
    cutAnimation.frames.push_back((Rectangle){ 0, 0, 527, 413 });  // Frame 0
    cutAnimation.frames.push_back((Rectangle){ 527, 0, 527, 413 });  // Frame 1
    cutAnimation.frames.push_back((Rectangle){ 0, 0, 527, 413 });  // Frame 0 again
    cutAnimation.currentFrame = 0;
    cutAnimation.frameTime = 0.025f;  // 25ms between frames
    cutAnimation.frameCounter = 0.0f;
    cutAnimation.isPlaying = false;
}

void Player::Init(float height) {
    playerPos.y = height;
    
    // Iniciar na esquerda
    playerPos.x = playerOffsetLeft;
    facingRight = false;
    hidden = false;
}

void Player::UpdateAnimation(Animation& anim, float deltaTime) {
    if (anim.isPlaying) {
        anim.frameCounter += deltaTime;
        
        if (anim.frameCounter >= anim.frameTime) {
            anim.frameCounter = 0.0f;
            anim.currentFrame++;
            
            if (anim.currentFrame >= anim.frames.size()) {
                anim.currentFrame = 0;
                
                // Se for a animação de corte, voltar para respiração
                if (&anim == &cutAnimation) {
                    isPlayerCutting = false;
                    cutAnimation.isPlaying = false;
                    breathAnimation.isPlaying = true;
                }
            }
        }
    }
}

void Player::Update(float deltaTime) {
    if (isPlayerCutting) {
        UpdateAnimation(cutAnimation, deltaTime);
    } else {
        UpdateAnimation(breathAnimation, deltaTime);
    }
}

void Player::CutLeft() {
    // Mover para esquerda
    playerPos.x = playerOffsetLeft;
    facingRight = false;
    isPlayerCutting = true;
    hidden = false;
    
    // Iniciar animação de corte
    cutAnimation.currentFrame = 0;
    cutAnimation.frameCounter = 0.0f;
    cutAnimation.isPlaying = true;
    breathAnimation.isPlaying = false;
}

void Player::CutRight() {
    // Mover para direita
    playerPos.x = playerOffsetRight;
    facingRight = true;
    isPlayerCutting = true;
    hidden = false;
    
    // Iniciar animação de corte
    cutAnimation.currentFrame = 0;
    cutAnimation.frameCounter = 0.0f;
    cutAnimation.isPlaying = true;
    breathAnimation.isPlaying = false;
}

void Player::Reset(float height) {
    playerPos.y = height;
    playerPos.x = playerOffsetLeft;
    facingRight = false;
    isPlayerCutting = false;
    hidden = false;
    
    // Resetar animações
    breathAnimation.currentFrame = 0;
    breathAnimation.frameCounter = 0.0f;
    breathAnimation.isPlaying = true;
    
    cutAnimation.currentFrame = 0;
    cutAnimation.frameCounter = 0.0f;
    cutAnimation.isPlaying = false;
}

void Player::Hide() {
    hidden = true;
}

bool Player::IsActionComplete() const {
    return !isPlayerCutting;
}

Side Player::GetSide() const {
    return facingRight ? Side::RIGHT : Side::LEFT;
}

Rectangle Player::GetCollisionRect() const {
    float scale = 0.6f;
    return {
        playerPos.x, 
        playerPos.y, 
        527 * scale, 
        413 * scale
    };
}

float Player::GetOffsetX() const {
    return facingRight ? playerOffsetRight : playerOffsetLeft;
}

void Player::Draw() const {
    if (!hidden) {
        Rectangle sourceRect;
        if (isPlayerCutting) {
            sourceRect = cutAnimation.frames[cutAnimation.currentFrame];
        } else {
            sourceRect = breathAnimation.frames[breathAnimation.currentFrame];
        }
        
        float scale = 0.8f;
        Rectangle destRect = {
            playerPos.x, 
            playerPos.y, 
            sourceRect.width * scale, 
            sourceRect.height * scale
        };
        
        if (facingRight) {

            DrawTexturePro(
                playerTexture,
                (Rectangle){sourceRect.x, sourceRect.y, -sourceRect.width, sourceRect.height},
                destRect,
                {0, 0},
                0.0f,
                WHITE
            );
        } else {

            DrawTexturePro(
                playerTexture,
                sourceRect,
                destRect,
                {0, 0},
                0.0f,
                WHITE
            );
        }
    }
}