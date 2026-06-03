#pragma once
#include <SFML/Network.hpp>
#include "GameRoom.h"
#include <queue>
#include <map>
#include "Utils.h"

#define MM MatchMakingManager::Instance()

class MatchMakingManager
{
public:
	static MatchMakingManager* Instance() {
		static MatchMakingManager nt;

		return &nt;
	}

private:
	std::map<GameMode, std::queue<Player>> waitingPlayers;

public:
	void AddConnectedPlayer(Player player, GameMode mode);

};

