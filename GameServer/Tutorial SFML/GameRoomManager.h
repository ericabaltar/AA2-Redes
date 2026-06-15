#pragma once 
#include <vector>
#include <iostream> 
#include <memory>
#include "Game.h" 
#include "Utils.h"
#include "NetworkManager.h"

#define GRM GameRoomManager::Instance()

class GameRoomManager {
public:
	static GameRoomManager* Instance() {
		static GameRoomManager manager;
		return &manager;
	}

private:
	std::vector<std::unique_ptr<GameRoom>> rooms;

	struct PendingPlayer
	{
		int roomId;
		Player player;
	};

	std::vector<PendingPlayer> pendingPlayers;

	GameRoom* FindRoom(int roomId)
	{
		for (auto& room : rooms)
		{
			if (room->GetId() == roomId)
				return room.get();
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
	GameRoom* CreateRoom(GameMode mode, int roomId) {
		std::cout << "Sala creada con id " << roomId << std::endl;
		rooms.push_back(std::make_unique<GameRoom>(roomId, mode));
		FlushPendingPlayers(roomId);
		return rooms.back().get();
	}

	void ConnectPlayerToRoom(int roomId, uint8_t playerIndex, const sf::IpAddress& ip, unsigned short port)
	{
		Player player(ip, port, playerIndex);
		GameRoom* room = FindRoom(roomId);

		if (room == nullptr)
		{
			std::cout << "Sala no existe aun. Guardando jugador en pending. RoomId: " << roomId << std::endl;
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

	GameRoom* FindRoomByUdp(const sf::IpAddress& ip, unsigned short port, int& outPlayerIndex)
	{
		for (auto& room : rooms)
		{
			for (int i = 0; i < room->GetPlayerAmount(); ++i) {
				Player* p = room->GetPlayer(i);
				if (p->udpIp.has_value() && p->udpIp.value() == ip && p->udpPort == port) {
					outPlayerIndex = i;
					return room.get();
				}
			}
		}
		return nullptr;
	}

	void HandleMovement(const sf::IpAddress& ip, unsigned short port, MovementPacket movement) {
		int playerIndex;
		GameRoom* room = FindRoomByUdp(ip, port, playerIndex);
		if (room) {
			room->HandleMovement(playerIndex, movement);
		}
	}

	void HandleShot(const sf::IpAddress& ip, unsigned short port, bool facingRight) {
		int playerIndex;
		GameRoom* room = FindRoomByUdp(ip, port, playerIndex);
		if (room) {
			room->HandleShot(playerIndex, facingRight);
		}
	}

	void HandleTaunt(const sf::IpAddress& ip, unsigned short port)
	{
		int playerIndex;
		GameRoom* room = FindRoomByUdp(ip, port, playerIndex);
		if (room) {
			room->HandleTaunt(playerIndex);
		}
	}

	void Update(float dt) {
		for (auto& room : rooms) {
			room->Update(dt);
		}
	}

private:
	GameRoomManager() = default;
	GameRoomManager(const GameRoomManager&) = delete;
	GameRoomManager& operator=(const GameRoomManager&) = delete;
};