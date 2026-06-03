#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "AnimatedSprite.h"

class Character
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


public:
    Character(const std::string& texturePath = "assets/grey_duck.png");

protected:
    void HandleAnimation(float dt);

public:
    virtual void Update(float dt);

    void Quack();

    void Shoot();

    inline sf::Sprite GetSprite() { return sprite.GetSprite(); }
    inline sf::Vector2f GetPosition() { return position; }
    inline void SetPosition(const sf::Vector2f newPosition) { position = newPosition; }
};

