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

    void HandleAnimation(float dt);

public:
    Character(const std::string& texturePath = "assets/grey_duck.png");

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

    virtual void Update(float dt, float groundY)
    {
        HandleAnimation(dt);
        sprite.SetPosition(position);
    }
};

