#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Game.h"
#include "Utils.h"

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

	GameRoom* FindRoom(int roomId)
	{
		for (GameRoom& room : rooms)
		{
			if (room.GetId() == roomId)
				return &room;
		}

		return nullptr;
	}

public:
	GameRoom* CreateRoom(GameMode mode, int roomId)
	{
		std::cout << "Sala creada con id " << roomId << std::endl;
		rooms.emplace_back(roomId, mode);

		return &rooms.back();
	}

	void ConnectPlayerToRoom(int roomId, uint8_t playerIndex, const sf::IpAddress& ip, unsigned short port)
	{/*
		Player player;
		player.index = playerIndex;
		player.udpIp = ip;
		player.udpPort = port;

		GameRoom* room = FindRoom(roomId);

		if (room == nullptr)
			std::cout << "Error al conectar jugador. No existe la sala con id " << roomId << std::endl;

		room->AddPlayer(player);

		if (room->CanStartGame())
			room->StartGame();*/
	}

private:
	GameRoomManager() = default;
	GameRoomManager(const GameRoomManager&) = delete;
	GameRoomManager& operator=(const GameRoomManager&) = delete;
};