#include "AnimatedSprite.h"
#include <iostream>

AnimatedSprite::AnimatedSprite(const std::string& textureDir)
	: sprite(texture)
{
	if (!texture.loadFromFile(textureDir))
		std::cout << "No se ha podido cargar el sprite." << std::endl;
}

void AnimatedSprite::FlipHorizontally(bool flip)
{
	if (flip)
		sprite.setScale({ -scale.x, scale.y });
	else
		sprite.setScale({ scale.x, scale.y });

}

void AnimatedSprite::AddAnimation(const std::string& animationName, const Animation& animation)
{
	animations.emplace(animationName, animation);
}

void AnimatedSprite::StartAnimation(const std::string& animationName, bool restartAnimation)
{
	auto it = animations.find(animationName);
	if (it == animations.end())
		return;

	Animation* newAnim = &it->second;

	if (!restartAnimation && currentAnimation == newAnim)
		return;

	currentAnimation = newAnim;
	currentAnimation->Reset();

	sprite.setTextureRect(currentAnimation->GetCurrentFrame());
	sprite.setOrigin({ currentAnimation->GetFrameWidth() / 2.f, currentAnimation->GetFrameHeight() / 2.f });
}

void AnimatedSprite::Update(float dt)
{
	currentAnimation->Update(dt);
	sprite.setTextureRect(currentAnimation->GetCurrentFrame());
}

sf::Sprite& AnimatedSprite::GetSprite()
{
	return sprite;
}

float AnimatedSprite::GetWidth()
{
	return sprite.getLocalBounds().size.x;
}

float AnimatedSprite::GetHeight()
{
	return sprite.getLocalBounds().size.y;
}

bool AnimatedSprite::IsCurrentAnimationFinished() const
{
	return currentAnimation && currentAnimation->HasFinished();
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


