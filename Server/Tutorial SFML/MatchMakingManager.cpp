#include "MatchMakingManager.h"
#include "NetworkManager.h"
#include <iostream>

bool MatchMakingManager::IsAvailableRoomId(std::string roomId)
{
	for (int i = 0; i < waitingRooms.size(); i++)
	{
		if (roomId == waitingRooms[i].GetId())
			return false;
	}
	return true;
}

Player* MatchMakingManager::GetPlayer(sf::TcpSocket* playerClient)
{
	for (int i = 0; i < connectedPlayers.size(); i++)
	{
		if (playerClient == connectedPlayers[i].client)
			return &connectedPlayers[i];
	}

	return nullptr;
}

GameRoom* MatchMakingManager::GetGameInfo(std::string roomId)
{
	for (int i = 0; i < waitingRooms.size(); i++)
	{
		if (roomId == waitingRooms[i].GetId())
			return &waitingRooms[i];
	}

	return nullptr;
}

void MatchMakingManager::AddConnectedPlayer(sf::TcpSocket* playerClient, std::string username, int playerPoints)
{
	connectedPlayers.push_back(Player(playerClient, username, playerPoints));
}

bool MatchMakingManager::CreateWaitingRoom(std::string roomId, sf::TcpSocket* playerClient)
{		
	if (!IsAvailableRoomId(roomId))
		return false;
	
	Player* player = GetPlayer(playerClient);

	if (player == nullptr)
	{
		std::cout << "Error: Jugador no encontrado";
		return false;
	}

	GameRoom room(roomId);
	room.AddPlayer(*player);

	waitingRooms.push_back(room);

	return true;
}

bool MatchMakingManager::JoinWaitingRoom(std::string roomId, sf::TcpSocket* playerClient)
{
	for (int i = 0; i < waitingRooms.size(); i++)
	{
		if (roomId == waitingRooms[i].GetId())
		{
			if (waitingRooms[i].IsFull()) return false;
			std::cout << "Es waiting room " << i << std::endl;
			waitingRooms[i].AddPlayer(*GetPlayer(playerClient));

			for (int j = 0; j < waitingRooms[i].GetPlayerAmount(); j++)
			{
				sf::TcpSocket* currentClient = waitingRooms[i].GetPlayer(j)->client;
				SPTM->SendUpdatedPlayerCount(*currentClient, waitingRooms[i].GetPlayerAmount());
				std::cout << "Matchmaking ha enviado mensaje al cliente " << j << " de la sala " << std::endl;
			}

			return true;
		}
	}
	return false;
}
