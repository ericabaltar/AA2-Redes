#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "GameStyle.h"
#include <functional>

class Button : public Object
{
public:
	sf::RectangleShape box;
	sf::Text label;
	std::function<void()> onClick;

	Button() : label(nullFont) {}

	Button(sf::RectangleShape _rect, sf::Text _text, std::function<void()> _onClick) : box(_rect), label(_text), onClick(_onClick) {}

	bool clickedInside(int posX, int posY) {
		sf::Vector2f boxPosition = box.getPosition();
		sf::Vector2f boxSize = box.getSize();
		return
			boxPosition.x <= posX && posX <= boxPosition.x + boxSize.x &&
			boxPosition.y <= posY && posY <= boxPosition.y + boxSize.y;
	}

	void Render(sf::RenderWindow& window) override {
		label.setPosition({
			box.getPosition().x + box.getSize().x / 2.0f - label.getCharacterSize() / 2.0f * label.getString().getSize() / 2.0f,
			box.getPosition().y + box.getSize().y / 2.0f - label.getCharacterSize() / 2.0f
			});
		window.draw(box);
		window.draw(label);
	}

	void HandleEvent(const sf::Event& event) override {
		if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {

			switch (mousePressed->button)
			{
			case sf::Mouse::Button::Left:
				if (clickedInside(mousePressed->position.x, mousePressed->position.y))
					onClick();
				break;
			default:
				break;
			}
		}
	}
};

