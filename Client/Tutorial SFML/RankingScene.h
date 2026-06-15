#pragma once
#include "Scene.h"
#include <vector>
#include "Ranking.h"
#include "NetworkManager.h"
#include "RankingManager.h"

class RankingScene : public Scene
{

	sf::Font* font;
	bool rankingReceived;
	float updateTimer;
	float updateDelay;

public:
	void Enter(SharedMemory* _sharedMemory) override {
		sharedMemory = _sharedMemory;
		font = new sf::Font(FONT_PATH);
		rankingReceived = false;

		NT->SendRankingPetitionServerPacket(16);
	}

	bool Update(sf::RenderWindow& window, float dt) override
	{
		for (Object* obj : objects) obj->Destroy();

		std::vector<User> users = RM->GetRanking();
		for (int index = 0; index < users.size(); ++index)
			objects.push_back(new Ranking(users[index], *font, index));

		return Scene::Update(window, dt);
	}
};

