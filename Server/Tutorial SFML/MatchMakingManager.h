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
	std::vector<Player> connectedPlayers;
	std::map<GameMode, std::queue<Player>> waitingPlayers;

	Player* GetPlayer(sf::TcpSocket* playerClient);

public:
	void AddPlayerToWaitingRoom(sf::TcpSocket* playerClient, GameMode mode);

	void AddConnectedPlayer(sf::TcpSocket* playerClient, std::string username, int playerPoints);
};

