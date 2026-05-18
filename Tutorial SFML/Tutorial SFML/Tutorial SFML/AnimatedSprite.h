#pragma once
#include <SFML/Graphics.hpp>

class AnimatedSprite
{
private:
	const int frameWidth = 17;
	const int frameHeight = 25;

	std::vector<sf::IntRect> frames;
	int currentFrame = 0;

	float timer = 0.f;
	float speed = 0.5f;

	bool isPlaying = false;

	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f scale = { 1.f, 1.f };

public:
	AnimatedSprite(std::string textureDir, int rows, int columns);
	void Start();
	void Update(float dt);
	void Stop();
	void FlipHorizontally(bool flip);

	sf::Sprite GetSprite();
	float GetWidth();
	float GetHeight();
	void SetPosition(sf::Vector2f pos);
	void SetScale(sf::Vector2f newScale);
};