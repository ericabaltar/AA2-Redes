#pragma once
#include <vector>
#include <string>
#include "Game.h"

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
	int nextRoomId = 0;

public:
	GameRoom* FindAvailableRoom(GameMode mode)
	{
		for (GameRoom& room : rooms)
		{
			if (room.GetGameMode() == mode && !room.IsFull() && !room.HasStarted())
				return &room;
		}

		return nullptr;
	}

	GameRoom* CreateRoom(GameMode mode)
	{
		int roomId = nextRoomId;
		nextRoomId++;

		rooms.emplace_back(roomId, mode);

		return &rooms.back();
	}

	GameRoom* JoinOrCreateRoom(Player& player, GameMode mode)
	{
		GameRoom* room = FindAvailableRoom(mode);

		if (room == nullptr)
			room = CreateRoom(mode);

		room->AddPlayer(player);

		if (room->CanStartGame())
			room->StartGame();

		return room;
	}

private:
	GameRoomManager() = default;
	GameRoomManager(const GameRoomManager&) = delete;
	GameRoomManager& operator=(const GameRoomManager&) = delete;
};