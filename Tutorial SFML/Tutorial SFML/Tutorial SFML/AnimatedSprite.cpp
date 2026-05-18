#include "AnimatedSprite.h"
#include <iostream>

AnimatedSprite::AnimatedSprite(std::string textureDir, int rows, int columns)
	: sprite(texture)
{
	if (!texture.loadFromFile(textureDir))
		std::cout << "No se ha podido cargar el sprite." << std::endl;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			sf::IntRect frame;

			frame.position.x = j * frameWidth;
			frame.position.y = i * frameHeight;

			frame.size.x = frameWidth;
			frame.size.y = frameHeight;

			frames.push_back(frame);
		}
	}

	sprite.setTextureRect(frames[0]);
	sprite.setOrigin({ frameWidth / 2.f, frameHeight / 2.f });
}

void AnimatedSprite::Start()
{
}

void AnimatedSprite::Update(float dt)
{
	timer += dt;

	if (timer >= speed)
	{
		timer = 0.f;
		currentFrame = (currentFrame + 1) % frames.size();
		sprite.setTextureRect(frames[currentFrame]);
	}
}

void AnimatedSprite::Stop()
{
	currentFrame = 0;
	sprite.setTextureRect(frames[0]);
}

void AnimatedSprite::FlipHorizontally(bool flip)
{
	if (flip)
		sprite.setScale({ -scale.x, scale.y });
	else
		sprite.setScale({ scale.x, scale.y });

}

sf::Sprite AnimatedSprite::GetSprite()
{
	return sprite;
}

float AnimatedSprite::GetWidth()
{
	return frameWidth;
}

float AnimatedSprite::GetHeight()
{
	return frameHeight;
}

void AnimatedSprite::SetPosition(sf::Vector2f pos)
{
	sprite.setPosition(pos);
}

void AnimatedSprite::SetScale(sf::Vector2f newScale)
{
	scale = newScale;
	sprite.setScale(scale);
}
