#pragma once
#include <SFML/Graphics.hpp>

class Object
{
public:
	virtual void update() {}
	virtual void render(sf::RenderWindow& window) {}
	virtual void handleEvent(const sf::Event& event) {}
};

