#pragma once
#include "Scene.h"
#include "config.h"
#include "InputField.h"
#include "Button.h"
#include "ServerPacketTypesManager.h"
#include "NetworkManager.h"
#include "LobbyManager.h"
#include <iostream>

class LobbyWaitingScene : public Scene
{
	sf::Font* arial;

	std::string id = "";
	int playerCount = 1;
	Button* playerAmountText;
	Button* roomTitle;

public:
	void Enter(SharedMemory* _sharedMemory) override {
		arial = new sf::Font("arial.ttf");

		// Title
		sf::RectangleShape titleRect;
		titleRect.setSize({ WINDOW_WIDTH * 0.8f, WINDOW_HEIGHT * 0.3f });
		titleRect.setPosition({ WINDOW_WIDTH * 0.49f - titleRect.getSize().x / 2.f, WINDOW_HEIGHT * 0.3f });
		titleRect.setFillColor(sf::Color::Transparent);
		std::string titleText = "LOBBY WAITING ROOM (" + id + ")";
		roomTitle = new Button(titleRect, sf::Text(*arial, titleText), []() {});

		objects.push_back(roomTitle);

		// Players
		sf::RectangleShape playerAmountRect;
		playerAmountRect.setSize({ WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.15f });
		playerAmountRect.setPosition({ WINDOW_WIDTH * 0.5f - playerAmountRect.getSize().x / 2.f, WINDOW_HEIGHT * 0.5f });
		playerAmountRect.setFillColor(sf::Color::Transparent);
		std::string playerCountText = std::to_string(playerCount) + "/4";
		playerAmountText = new Button(playerAmountRect, sf::Text(*arial, playerCountText), []() {});

		objects.push_back(playerAmountText);
	}

	bool Update(sf::RenderWindow& window, float dt) override
	{
		if (LM->GetGameStarted())
			nextScene = SceneOption::GAME;

		return Scene::Update(window, dt);
	}
};

