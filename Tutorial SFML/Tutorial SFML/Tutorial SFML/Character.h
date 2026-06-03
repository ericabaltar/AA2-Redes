#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "AnimatedSprite.h"
#include "Object.h"

class Character : public Object
{
protected:
    sf::SoundBuffer quackBuffer;
    sf::Sound quackSound;

	const float scale = 4.f;
    float width;
    float height;

    AnimatedSprite sprite;

	Vector2 velocity;

	const float jumpBufferTime = 0.12f;
	float jumpBufferTimer = 0.f;

	bool isOnGround = false;

	bool facingRight = true;

    bool isQuacking = false;
    bool isShooting = false;

    Vector2 colliderOffset;

    const int lives = 3;
    const int healthPoints = 5;
    int currentLives = lives;
    int currentHealthPoints = healthPoints;

public:
    Character(const std::string& texturePath = "assets/grey_duck.png");

protected:
    void HandleAnimation(float dt);

    void ReceiveHit();

public:
    void Update(float dt) override;

    void Quack();

    void Shoot();

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
    void Render(sf::RenderWindow& window) override { window.draw(sprite.GetSprite()); }
    
    inline bool IsDead() { return lives <= 0; }

    inline Vector2 GetPosition() { return transform->position; }
    inline void SetPosition(const Vector2 newPosition) { transform->position = newPosition; }
    void HandleEvent(const sf::Event& event) override {}
};

