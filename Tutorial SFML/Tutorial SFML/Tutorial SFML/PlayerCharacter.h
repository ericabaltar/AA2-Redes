#pragma once
#include "Character.h"

class PlayerCharacter : public Character
{
private:
	float inputX = 0;

    const float acceleration = 1800.f;
    const float maxSpeed = 1500.f;
    const float friction = 8.f;

    const float gravity = 2000.f;
    const float jumpForce = -900.f;

    const float coyoteTime = 0.12f;
    float coyoteTimer = 0.f;

    void HandleInput(float dt);

    void ApplyPhysics(float dt, float groundY);


public:
    PlayerCharacter(): Character("assets/white_duck.png") {
        position = { 100.f, 300.f };
    }

    void Update(float dt) override; 

    inline void ResetJumpBuffer() { jumpBufferTimer = jumpBufferTime; }

};

