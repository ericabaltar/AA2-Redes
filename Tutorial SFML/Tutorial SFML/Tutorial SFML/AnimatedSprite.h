#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Animation.h"

class AnimatedSprite
{
private:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f scale = { 1.f, 1.f };

	std::unordered_map<std::string, Animation> animations;
	Animation* currentAnimation = nullptr;
	std::string currentAnimationName;

public:
	AnimatedSprite(const std::string& textureDir);
	
	void AddAnimation(const std::string& animationName, const Animation& animation);
	void StartAnimation(const std::string& animationName, bool restartAnimation = false);
	void Update(float dt);

	sf::Sprite& GetSprite();
	float GetWidth();
	float GetHeight();
	bool IsCurrentAnimationFinished() const;

	void SetPosition(sf::Vector2f pos);
	void SetScale(sf::Vector2f newScale);
	void FlipHorizontally(bool flip);
};