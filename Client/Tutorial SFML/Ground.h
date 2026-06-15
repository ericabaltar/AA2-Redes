#pragma once
#include "Object.h"

class Ground : public Object
{
private:
	sf::RectangleShape shape;

public:
	Ground(Vector2 pos, Vector2 size);

	void Update(float dt) override;
	void Render(sf::RenderWindow& window) override;
};

