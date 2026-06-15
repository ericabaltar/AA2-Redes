#pragma once 
#include <vector> 
#include <iostream> 
#include <optional> 
#include "Game.h" 
#include "Utils.h"
#include "NetworkManager.h"

#define GRM GameRoomManager::Instance()

class GameRoomManager
{
public:
	static GameRoomManager* Instance()
	{
		static GameRoomManager manager;
		return &manager;
	}

private:
	std::vector<GameRoom> rooms;

	struct PendingPlayer
	{
		int roomId;
		Player player;
	};

	std::vector<PendingPlayer> pendingPlayers;

	GameRoom* FindRoom(int roomId)
	{
		for (GameRoom& room : rooms)
		{
			if (room.GetId() == roomId)
				return &room;
		}

		return nullptr;
	}

	void FlushPendingPlayers(int roomId)
	{
		GameRoom* room = FindRoom(roomId);
		if (room == nullptr)
			return;

		for (std::vector<PendingPlayer>::iterator it = pendingPlayers.begin(); it != pendingPlayers.end(); )
		{
			if (it->roomId == roomId)
			{
				room->AddPlayer(it->player);
				it = pendingPlayers.erase(it);
			}
			else
			{
				++it;
			}
		}

		if (room->CanStartGame())
		{
			room->StartGame();
			for (int i = 0; i < room->GetPlayerAmount(); ++i) {
				Player* p = room->GetPlayer(i);
				NT->GetUdpManager()->SendMatchStart(p->udpIp.value(), p->udpPort);
			}
		}
	}

public:
	GameRoom* CreateRoom(GameMode mode, int roomId)
	{
		std::cout << "Sala creada con id " << roomId << std::endl;
		rooms.emplace_back(roomId, mode);

		FlushPendingPlayers(roomId);

		return &rooms.back();
	}

	void ConnectPlayerToRoom(int roomId, uint8_t playerIndex, const sf::IpAddress& ip, unsigned short port)
	{
		Player player;
		player.index = playerIndex;
		player.udpIp = ip;
		player.udpPort = port;

		GameRoom* room = FindRoom(roomId);

		if (room == nullptr)
		{
			std::cout << "Sala no existe an. Guardando jugador en pending. RoomId: " << roomId << std::endl;

			pendingPlayers.push_back({ roomId, player });
			return;
		}

		room->AddPlayer(player);

		if (room->CanStartGame())
		{
			room->StartGame();
			for (int i = 0; i < room->GetPlayerAmount(); ++i) {
				Player* p = room->GetPlayer(i);
				NT->GetUdpManager()->SendMatchStart(p->udpIp.value(), p->udpPort);
			}
		}
	}

private:
	GameRoomManager() = default;
	GameRoomManager(const GameRoomManager&) = delete;
	GameRoomManager& operator=(const GameRoomManager&) = delete;
};