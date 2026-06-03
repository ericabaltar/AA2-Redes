#pragma once
#include "Scene.h"
#include <vector>
#include "Ranking.h"
#include "NetworkManager.h"

class RankingScene : public Scene
{

	sf::Font* font;
	bool rankingReceived;
	float updateTimer;
	float updateDelay;

public:
	void enter(SharedMemory* _sharedMemory) {
		sharedMemory = _sharedMemory;
		font = new sf::Font(FONT_PATH);
		rankingReceived = false;

		NT->SendRankingPetitionServerPacket(16);

		std::vector<User> users;
		users = SPTM->GetRanking();

		for (int index = 0; index < users.size(); ++index)
			objects.push_back(new Ranking(users[index], *font, index));
	}
};

