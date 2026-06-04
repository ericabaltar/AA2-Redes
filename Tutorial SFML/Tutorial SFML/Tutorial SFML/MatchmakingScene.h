#pragma once
#include "Scene.h"
#include "Button.h"
#include "Utils.h"

class MatchmakingScene : public Scene
{
	sf::Font* font;

	Button* friendlyMatchButton;
	Button* competitiveMatchButton;

public:
	void Enter(SharedMemory* _sharedMemory) override {
		font = new sf::Font("arial.ttf");
		sharedMemory = _sharedMemory;

		// Title
		sf::RectangleShape competitiveMatchRect;
		competitiveMatchRect.setSize({ WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.15f });
		competitiveMatchRect.setPosition({ WINDOW_WIDTH * .5f - competitiveMatchRect.getSize().x / 2.f, WINDOW_HEIGHT * 0.33f });
		competitiveMatchRect.setFillColor(sf::Color::Red);
		std::string competitiveMatchText = "Competitive Match";
		competitiveMatchButton = new Button(competitiveMatchRect, sf::Text(*font, competitiveMatchText), [&]() {
			sharedMemory->saveInt("MatchType", (int)GameMode::COMPETITIVE);
			nextScene = SceneOption::WAITING_ROOM;
			});

		objects.push_back(competitiveMatchButton);

		// Players
		sf::RectangleShape friendlyMatchRect;
		friendlyMatchRect.setSize({ WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.15f });
		friendlyMatchRect.setPosition({ WINDOW_WIDTH * .5f - friendlyMatchRect.getSize().x / 2.f, WINDOW_HEIGHT * 0.66f });
		friendlyMatchRect.setFillColor(sf::Color::Blue);
		std::string friendlyMatchText = "Friendly Match";
		friendlyMatchButton = new Button(friendlyMatchRect, sf::Text(*font, friendlyMatchText), [&]() {
			sharedMemory->saveInt("MatchType", (int)GameMode::FRIENDLY);
			nextScene = SceneOption::WAITING_ROOM;
			});

		objects.push_back(friendlyMatchButton);
	}
};

