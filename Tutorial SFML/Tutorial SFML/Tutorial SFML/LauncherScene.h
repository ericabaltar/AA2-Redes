#pragma once
#include "Scene.h"
#include "Button.h"
#include "config.h"
#include "MapManager.h"
#include "NetworkManager.h"
class LauncherScene : public Scene
{

	sf::Font* arial;

	float timeElapsed = 0.0f;
	Button* welcomeText;
	Button* retrievingText;
	Button* timeText;
	Button* enterButton;
	bool isEnterVisible = false;

public:
	void Enter(SharedMemory* _sharedMemory) override {
		arial = new sf::Font("arial.ttf");

		sf::RectangleShape welcomeRect;
		welcomeRect.setSize({ WINDOW_WIDTH * 0.8f, WINDOW_HEIGHT * 0.15f });
		welcomeRect.setPosition({ WINDOW_WIDTH * .5f - welcomeRect.getSize().x / 2.f, WINDOW_HEIGHT * 0.15f });
		welcomeRect.setFillColor(sf::Color::Transparent);
		std::string welcomeLabel = "Welcome to UDP Shooter";
		welcomeText = new Button(welcomeRect, sf::Text(*arial, welcomeLabel, 70U), []() {});

		objects.push_back(welcomeText);

		
		sf::RectangleShape retrieveRect;
		retrieveRect.setSize({ WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.15f });
		retrieveRect.setPosition({ WINDOW_WIDTH * 0.5f - retrieveRect.getSize().x / 2.f, WINDOW_HEIGHT * 0.3f });
		retrieveRect.setFillColor(sf::Color::Transparent);
		std::string retrieveLabel = "Checking map...";
		retrievingText = new Button(retrieveRect, sf::Text(*arial, retrieveLabel), []() {});

		objects.push_back(retrievingText);

		sf::RectangleShape timeRect;
		timeRect.setSize({ WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.15f });
		timeRect.setPosition({ WINDOW_WIDTH * 0.5f - timeRect.getSize().x / 2.f, WINDOW_HEIGHT * 0.35f});
		timeRect.setFillColor(sf::Color::Transparent);
		std::string timeLabel = "Time elapsed: " + std::to_string((int)std::floor(timeElapsed));
		timeText = new Button(timeRect, sf::Text(*arial, timeLabel), []() {});

		objects.push_back(timeText);

		sf::RectangleShape enterRect;
		enterRect.setSize({ WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.15f });
		enterRect.setPosition({ WINDOW_WIDTH * 0.5f - enterRect.getSize().x / 2.f, WINDOW_HEIGHT * .7f });
		enterRect.setFillColor(sf::Color::Blue);
		std::string enterLabel = "Enter";
		enterButton = new Button(enterRect, sf::Text(*arial, enterLabel, 50U), [&]() { nextScene = SceneOption::LOGIN; });

		NT->SendMapPetitionServerPacket();
	}

	bool Update(sf::RenderWindow& window, float dt) override
	{
		if (!MapM->GetDirtyState() && !isEnterVisible) {
			objects.push_back(enterButton);
			isEnterVisible = true;
		}

		timeElapsed += dt;
		timeText->label.setString("Time elapsed: " + std::to_string((int)std::floor(timeElapsed)));

		return Scene::Update(window, dt);
	}
};

