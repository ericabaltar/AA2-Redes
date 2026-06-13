#include "PlayerCharacter.h"
#include "NetworkManager.h"
#include "Ground.h"

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

void PlayerCharacter::ApplyPhysics(float dt)
{
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

    isOnGround = false;
}

void PlayerCharacter::HandleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
            ResetJumpBuffer();
    }
}

void PlayerCharacter::Update(float dt)
{
    HandleInput(dt);
    ApplyPhysics(dt);
    Character::Update(dt);
}

void PlayerCharacter::Quack()
{
    Character::Quack();
    NT->SendTaunt();
}

void PlayerCharacter::Shoot()
{
    Character::Shoot();
}

void PlayerCharacter::OnCollisionEnter(Object* other, const CollisionInfo& collisionInfo)
{
    Character::OnCollisionEnter(other, collisionInfo);

    if (dynamic_cast<Ground*>(other))
    {
        if (collisionInfo.normal.y < 0.f)
        {
            isOnGround = true;
            coyoteTimer = coyoteTime;
        }
    }
}
