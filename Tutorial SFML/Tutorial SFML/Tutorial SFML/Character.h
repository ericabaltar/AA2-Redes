#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "AnimatedSprite.h"

class Character
{
private:
    sf::SoundBuffer quackBuffer;
    sf::Sound quackSound;

	const float scale = 4.f;
    float width;
    float height;

    AnimatedSprite sprite;

	sf::Vector2f position;
	sf::Vector2f velocity;

	const float acceleration = 1800.f;
	const float maxSpeed = 1500.f;
	const float friction = 8.f;

	const float gravity = 2000.f;
	const float jumpForce = -900.f;

	const float coyoteTime = 0.12f;
	const float jumpBufferTime = 0.12f;

	float coyoteTimer = 0.f;
	float jumpBufferTimer = 0.f;

	bool isOnGround = false;

	bool facingRight = true;

    float inputX = 0;

    bool isQuacking = false;

    void HandleInput(float dt)
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

    void ApplyPhysics(float dt, float groundY)
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

    void HandleAnimation(float dt)
    {
        if (isQuacking)
        {
            sprite.Update(dt);

            if (sprite.IsCurrentAnimationFinished())
            {
                isQuacking = false;
                sprite.StartAnimation("idle", true);
            }

            return;
        }

        bool isMoving = std::abs(velocity.x) > 20.f;

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

public:
	Character()
		: sprite("assets/grey_duck.png"), quackSound(quackBuffer)
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

    sf::Sprite GetSprite()
    {
        return sprite.GetSprite();
    }

    sf::Vector2f GetPosition()
    {
        return position;
    }

    void ResetJumpBuffer()
    {
        jumpBufferTimer = jumpBufferTime;
    }

    void Update(float dt, float groundY)
    {
        HandleInput(dt);
        ApplyPhysics(dt, groundY);
        HandleAnimation(dt);
        sprite.SetPosition(position);
    }

    sf::Vector2f GetPosition()
    {
        return position;
    }

    void SetPosition(sf::Vector2f newPosition)
    {
        position = newPosition;
        sprite.SetPosition(position);
    }
};

