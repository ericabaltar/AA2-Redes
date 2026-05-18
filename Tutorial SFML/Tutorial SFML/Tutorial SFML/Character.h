#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Character
{
private:
	const int frameWidth = 15;
	const int frameHeight = 25;
	const float scale = 4.f;

	sf::Texture texture;
	sf::Sprite sprite;

    std::vector<sf::IntRect> walkFrames;

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

	float animationTimer = 0.f;
	const float animationSpeed = 0.08f;
	int currentFrame = 0;

	bool facingRight = true;

    float inputX;

public:
	Character()
		: sprite(texture)
	{
		if (!texture.loadFromFile("assets/grey_duck_movement.png"))
			std::cout << "No se ha podido cargar el sprite." << std::endl;

		walkFrames =
		{
			sf::IntRect({ 0, 0 },   { frameWidth, frameHeight }),
			sf::IntRect({ 32, 0 },  { frameWidth, frameHeight }),
			sf::IntRect({ 64, 0 },  { frameWidth, frameHeight }),
			sf::IntRect({ 96, 0 },  { frameWidth, frameHeight }),
			sf::IntRect({ 128, 0 }, { frameWidth, frameHeight }),
			sf::IntRect({ 160, 0 }, { frameWidth, frameHeight })
		};

		sprite.setTextureRect(walkFrames[0]);
		sprite.setOrigin({ frameWidth / 2.f, frameHeight / 2.f });
		sprite.setPosition({ 400.f, 300.f });
		sprite.setScale({ scale, scale });

		velocity = sf::Vector2f(0, 0);
	}

    sf::Sprite GetSprite()
    {
        return sprite;
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

        sprite.move(velocity * dt);

        position = sprite.getPosition();

        float duckWidth = frameWidth * scale;
        float duckHeight = frameHeight * scale;

        if (position.y + duckHeight / 2.f >= groundY)
        {
            sprite.setPosition({ position.x, groundY - duckHeight / 2.f });
            velocity.y = 0.f;
            isOnGround = true;
        }
        else
        {
            isOnGround = false;
        }

        position = sprite.getPosition();

        if (position.x - duckWidth / 2.f < 0.f)
        {
            sprite.setPosition({ duckWidth / 2.f, position.y });
            velocity.x = 0.f;
        }

        if (position.x + duckWidth / 2.f > 800.f)
        {
            sprite.setPosition({ 800.f - duckWidth / 2.f, position.y });
            velocity.x = 0.f;
        }

        bool isMoving = std::abs(velocity.x) > 20.f;

        if (velocity.x > 5.f)
            facingRight = true;
        else if (velocity.x < -5.f)
            facingRight = false;

        if (isMoving)
        {
            animationTimer += dt;

            if (animationTimer >= animationSpeed)
            {
                animationTimer = 0.f;
                currentFrame = (currentFrame + 1) % walkFrames.size();
                sprite.setTextureRect(walkFrames[currentFrame]);
            }
        }
        else
        {
            currentFrame = 0;
            sprite.setTextureRect(walkFrames[0]);
        }

        if (facingRight)
            sprite.setScale({ scale, scale });
        else
            sprite.setScale({ -scale, scale });
	}
};

