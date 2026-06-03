#include "PlayerCharacter.h"

void PlayerCharacter::HandleInput(float dt)
{
    bool isDoingAction = isQuacking || isShooting;
    if (isOnGround)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) && !isDoingAction)
            Quack();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) && !isDoingAction)
            Shoot();
    }

    if (isDoingAction) return;

    inputX = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        inputX -= 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        inputX += 1.f;
}

void PlayerCharacter::ApplyPhysics(float dt, float groundY)
{
    std::cout << "Posicion inicial: " << transform->position.x << "   " << transform->position.y << std::endl;

    if (isQuacking || isShooting)
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

    if (jumpBufferTimer > 0.f && coyoteTimer > 0.f && !isQuacking && !isShooting)
    {
        velocity.y = jumpForce;
        jumpBufferTimer = 0.f;
        coyoteTimer = 0.f;
        isOnGround = false;
    }

    velocity.y += gravity * dt;
    transform->position = transform->position + velocity * dt;

    if (transform->position.y + width / 2.f >= groundY)
    {
        transform->position = { transform->position.x, groundY - height / 2.f };
        velocity.y = 0.f;
        isOnGround = true;
    }
    else
    {
        isOnGround = false;
    }

    if (transform->position.x - width / 2.f < 0.f)
    {
        transform->position = { width / 2.f, transform->position.y };
        velocity.x = 0.f;
    }

    if (transform->position.x + width / 2.f > 800.f)
    {
        transform->position = { 800.f - width / 2.f, transform->position.y };
        velocity.x = 0.f;
    }

    std::cout << "Posicion final: " << transform->position.x << "   " << transform->position.y << std::endl;

}

void PlayerCharacter::Update(float dt)
{
    float groundY = 550; // PENDIENTE QUITAR ESTO CUANDO HAYA COLISIONES REALES CON SUELO POR TILES

    HandleInput(dt);
    ApplyPhysics(dt, groundY);
    Character::Update(dt);
}
