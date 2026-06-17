#pragma once
#include <SFML/Graphics.hpp>

class Animation
{
private:
	std::vector<sf::IntRect> frames;
	int currentFrame = 0;

	float timer = 0.f;
	float frameTime = 0.1f;

	bool loop = true;
	bool finished = false;

public:
	Animation(int frameWidth, int frameHeight, int row, int columns, float frameTime, bool loop = true);

	void Update(float dt);
	void Reset();

	const sf::IntRect& GetCurrentFrame() const;
	bool HasFinished() const;

	int GetFrameWidth() const;
	int GetFrameHeight() const;
};

