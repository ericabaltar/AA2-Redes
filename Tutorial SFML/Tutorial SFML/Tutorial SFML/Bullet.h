#pragma once
#include "Object.h"
#include <iostream>

class Bullet : public Object
{
private:
	sf::Texture texture;
	sf::Sprite sprite;

	const float scale = 4.f;
	float width;
	float height;

	sf::Vector2f position;
	sf::Vector2f velocity;

public:
	Bullet(const std::string& texturePath = "assets/bullet.png")
		: texture(texturePath), sprite(texture)
	{
		position = { 0.f, 500.f };
		sprite.setScale({ scale, scale });

		velocity = sf::Vector2f(700.f, 0);
		width = sprite.getLocalBounds().size.x * scale;
		height = sprite.getLocalBounds().size.y * scale;
	}

	void Update(float dt) override
	{
		position += velocity * dt;
		sprite.setPosition(position);
	}

	void Render(sf::RenderWindow& window) override
	{
		window.draw(sprite);
	}
};

