#include "PlayerCharacter.h"

void PlayerCharacter::HandleInput(float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) && !isQuacking && isOnGround)
    {
        quackSound.play();
        isQuacking = true;
        sprite.StartAnimation("quack", true);
    }

    if (isQuacking)
    {
        inputX = 0.f;
        return;
    }

    inputX = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        inputX -= 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        inputX += 1.f;
}

void PlayerCharacter::ApplyPhysics(float dt, float groundY)
{
    if (isQuacking)
    {
        velocity.x = 0.f;
    }
    else
    {
        velocity.x += inputX * acceleration * dt;
        velocity.x *= 1.f / (1.f + friction * dt);

        if (std::abs(velocity.x) > maxSpeed)
            velocity.x = (velocity.x > 0.f ? 1.f : -1.f) * maxSpeed;

        if (velocity.x > 5.f)
            facingRight = true;
        else if (velocity.x < -5.f)
            facingRight = false;
    }

    if (isOnGround)
        coyoteTimer = coyoteTime;
    else
        coyoteTimer -= dt;

    jumpBufferTimer -= dt;

    if (jumpBufferTimer > 0.f && coyoteTimer > 0.f && !isQuacking)
    {
        velocity.y = jumpForce;
        jumpBufferTimer = 0.f;
        coyoteTimer = 0.f;
        isOnGround = false;
    }

    velocity.y += gravity * dt;
    position += velocity * dt;

    if (position.y + width / 2.f >= groundY)
    {
        position = { position.x, groundY - height / 2.f };
        velocity.y = 0.f;
        isOnGround = true;
    }
    else
    {
        isOnGround = false;
    }

    if (position.x - width / 2.f < 0.f)
    {
        position = { width / 2.f, position.y };
        velocity.x = 0.f;
    }

    if (position.x + width / 2.f > 800.f)
    {
        position = { 800.f - width / 2.f, position.y };
        velocity.x = 0.f;
    }
}