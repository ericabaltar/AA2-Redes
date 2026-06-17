#include "Ground.h"

Ground::Ground(Vector2 pos, Vector2 size)
    : shape({size.x, size.y})
{
    shape.setFillColor(sf::Color(100, 100, 100));
    shape.setPosition({pos.x, pos.y});

    transform->position = Vector2(
        pos.x + size.x * 0.5f,
        pos.y + size.y * 0.5f
    );

    collider->SetTopLeft(Vector2(pos.x, pos.y));
    collider->SetSize(Vector2(size.x, size.y));
}

void Ground::Update(float dt)
{
}

void Ground::Render(sf::RenderWindow& window)
{
	window.draw(shape);
}
