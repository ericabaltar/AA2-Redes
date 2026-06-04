#include "MatchMakingManager.h"
#include "NetworkManager.h"
#include <iostream>
#include "GameRoom.h"
#include "GameManager.h"

void MatchMakingManager::AddPlayerToWaitingRoom(sf::TcpSocket* playerClient, GameMode mode)
{
	Player* player = GetPlayer(playerClient);

	std::cout << "Player añadido a la cola" << std::endl;
	if (waitingPlayers[mode].empty()) {
		waitingPlayers[mode].push(*player);
		return;
	}

	Player other = waitingPlayers[mode].front();
	waitingPlayers[mode].pop();

	GM->AddRoom(GameRoom(other, *player, mode));
}

Player* MatchMakingManager::GetPlayer(sf::TcpSocket* playerClient)
{
	for (int i = 0; i < connectedPlayers.size(); i++)
	{
		if (playerClient == connectedPlayers[i].client)
			return &connectedPlayers[i];
	}

	// TEMPORAL: Crea un player generico para no tener que iniciar sesión
	return new Player(playerClient, "Player", 0);

	return nullptr;
}

void MatchMakingManager::AddConnectedPlayer(sf::TcpSocket* playerClient, std::string username, int playerPoints)
{
	connectedPlayers.push_back(Player(playerClient, username, playerPoints));
}
