#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "RankingStyle.h"
#include "User.h"
#include <iostream>

class Ranking : public Object
{
	sf::RectangleShape rectangle;
	sf::Text text;

public:
	Ranking() : text(emptyFont) {}

	Ranking(User user, sf::Font& font, int index) : text(font) {
		rectangle.setPosition({
			WINDOW_WIDTH * rankingsAnchorX,
			WINDOW_HEIGHT * rankingsAnchorY + user.userIndex * (rankingsHeight + rankingsSeparation)
			});
		rectangle.setFillColor(rankingsBackground);
		rectangle.setSize({ rankingsWidth, rankingsHeight });

		text.setString(user.nickname + "             " + std::to_string(user.score));

		//Center text
		text.setPosition({
			rectangle.getPosition().x + rectangle.getSize().x / 2.0f - text.getCharacterSize() / 2.0f * text.getString().getSize() / 2.0f,
			rectangle.getPosition().y + rectangle.getSize().y / 2.0f - text.getCharacterSize() / 2.0f
			});

		text.setCharacterSize(rankingsCharacterSize);
		text.setStyle(rankingsTextStyle);
		text.setFillColor(sf::Color::Black);
	}

	void render(sf::RenderWindow& window) {
		window.draw(rectangle);
		window.draw(text);
	}
};

