#pragma once
#include <SFML/Network.hpp>
#include "Game.h"

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
	std::vector<GameRoom> waitingRooms;

	bool IsAvailableRoomId(std::string roomId);	
	Player* GetPlayer(sf::TcpSocket* playerClient);
	GameRoom* GetGameInfo(std::string roomId);

public:
	void AddConnectedPlayer(sf::TcpSocket* playerClient, std::string username, int playerPoints);
	bool CreateWaitingRoom(std::string roomId, sf::TcpSocket* playerClient);
	bool JoinWaitingRoom(std::string roomId, sf::TcpSocket* playerClient);

};

