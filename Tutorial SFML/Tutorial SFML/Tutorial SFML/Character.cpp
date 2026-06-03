#include "Character.h"

void Character::HandleAnimation(float dt)
{
    if (isQuacking || isShooting)
    {
        sprite.Update(dt);

        if (sprite.IsCurrentAnimationFinished())
        {
            isQuacking = false;
            isShooting = false;
            sprite.StartAnimation("idle", true);
        }

        return;
    }

    bool isMoving = std::abs(velocity.x) >= walkAnimVelocityThreshold;

    if (isMoving)
        sprite.StartAnimation("walk");
    else
        sprite.StartAnimation("idle");

    if (facingRight)
        sprite.FlipHorizontally(false);
    else
        sprite.FlipHorizontally(true);

    sprite.Update(dt);
}

Character::Character(const std::string& texturePath) : sprite(texturePath), quackSound(quackBuffer)
{
    position = { 400.f, 300.f };
    sprite.SetScale({ scale, scale });

    sprite.AddAnimation("idle", Animation(32, 32, 0, 1, 0));
    sprite.AddAnimation("walk", Animation(32, 32, 1, 6, 0.1f));
    sprite.AddAnimation("shoot", Animation(32, 32, 2, 5, 0.05f, false));
    sprite.AddAnimation("quack", Animation(32, 32, 3, 1, 0.5f, false));

    sprite.StartAnimation("idle");

    velocity = sf::Vector2f(0, 0);

    width = sprite.GetWidth() * scale;
    height = sprite.GetHeight() * scale;

    if (!quackBuffer.loadFromFile("assets/quack.wav"))
        std::cout << "Error cargando quack.wav" << std::endl;

    quackSound.setVolume(20.f);
}

void Character::Update(float dt)
{
    HandleAnimation(dt);
    sprite.SetPosition(position);
}

void Character::Quack()
{
    isQuacking = true;
    sprite.StartAnimation("quack");

    quackSound.play();
}

void Character::Shoot()
{
    isShooting = true;
    sprite.StartAnimation("shoot");

    // Shoot
}

void Character::SetInterpolatedPosition(const sf::Vector2f newPosition)
{
    if (newPosition.x > position.x || newPosition.x < position.x) {
		velocity.x = walkAnimVelocityThreshold;
    }
    else
    {
        velocity.x = 0.f;
    }
        
    position = newPosition;
}
