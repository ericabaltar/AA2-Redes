#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "AnimatedSprite.h"

class Character
{
private:
	const float scale = 4.f;
    float width;
    float height;

    AnimatedSprite sprite;

	sf::Vector2f position;
	sf::Vector2f velocity;

	const float acceleration = 1200.f;
	const float maxSpeed = 300.f;
	const float friction = 8.f;

	const float gravity = 2000.f;
	const float jumpForce = -700.f;

	const float coyoteTime = 0.12f;
	const float jumpBufferTime = 0.12f;

	float coyoteTimer = 0.f;
	float jumpBufferTimer = 0.f;

	bool isOnGround = false;

	bool facingRight = true;

    float inputX;

public:
	Character()
		: sprite("assets/grey_duck_movement.png", 1, 5)
	{
		position = { 400.f, 300.f };
		sprite.SetScale({ scale, scale });

		velocity = sf::Vector2f(0, 0);

        width = sprite.GetWidth() * scale;
        height = sprite.GetHeight() * scale;
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

    void DetectInput()
    {
        inputX = 0.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            inputX -= 1.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            inputX += 1.f;
    }

	void Update(float dt, float groundY)
	{
        velocity.x += inputX * acceleration * dt;
        velocity.x *= 1.f / (1.f + friction * dt);

        if (std::abs(velocity.x) > maxSpeed)
            velocity.x = (velocity.x > 0.f ? 1.f : -1.f) * maxSpeed;

        if (isOnGround)
            coyoteTimer = coyoteTime;
        else
            coyoteTimer -= dt;

        jumpBufferTimer -= dt;

        if (jumpBufferTimer > 0.f && coyoteTimer > 0.f)
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

        bool isMoving = std::abs(velocity.x) > 20.f;

        if (velocity.x > 5.f)
            facingRight = true;
        else if (velocity.x < -5.f)
            facingRight = false;

        if (isMoving)
        {
            sprite.Update(dt);
        }
        else
        {
            sprite.Stop();
        }

        if (facingRight)
            sprite.FlipHorizontally(false);
        else
            sprite.FlipHorizontally(true);

        sprite.SetPosition(position);
	}
};

