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

	sf::Vector2f position;
	sf::Vector2f velocity;

	const float jumpBufferTime = 0.12f;
	float jumpBufferTimer = 0.f;

	bool isOnGround = false;

	bool facingRight = true;

    bool isQuacking = false;
    bool isShooting = false;

    float walkAnimVelocityThreshold = 20.f;


public:
    Character(const std::string& texturePath = "assets/grey_duck.png");

protected:
    void HandleAnimation(float dt);

public:
    void Update(float dt) override;

    void Quack();

    void Shoot();

    void Render(sf::RenderWindow& window) override { window.draw(sprite.GetSprite()); }
    
    inline sf::Vector2f GetPosition() { return position; }
    void SetInterpolatedPosition(const sf::Vector2f newPosition);
    void HandleEvent(const sf::Event& event) override {}
};

