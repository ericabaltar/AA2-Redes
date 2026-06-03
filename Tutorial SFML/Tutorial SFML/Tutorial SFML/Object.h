#pragma once
#include <SFML/Graphics.hpp>

class Object
{
public:
	virtual void Update(float dt) {}
	virtual void Render(sf::RenderWindow& window) {}
	virtual void HandleEvent(const sf::Event& event) {}
};

