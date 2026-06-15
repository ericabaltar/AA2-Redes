#pragma once
#include <string>
#include "Player.h"
#include "Utils.h"

class GameRoom
{
public:
	GameRoom(int roomId, GameMode mode)
		: id(roomId), gameMode(mode) {
	}

private:
	int id;
	GameMode gameMode;

	static const int maxPlayers = 2;

	Player players[maxPlayers];

	int playersAdded = 0;
	bool gameStarted = false;

public:
	int GetId()
	{
		return id;
	}

	GameMode GetGameMode()
	{
		return gameMode;
	}

	bool AddPlayer(Player& player)
	{
		if (IsFull())
			return false;

		players[playersAdded] = player;
		playersAdded++;

		return true;
	}

	Player* GetPlayer(int index)
	{
		if (index >= 0 && index < playersAdded)
			return &players[index];

		return nullptr;
	}

	int GetPlayerAmount()
	{
		return playersAdded;
	}

	bool IsFull()
	{
		return playersAdded == maxPlayers;
	}

	// Una partida només pot iniciar-se quan la sala està completa
	// i encara no s'ha marcat com iniciada.
	bool CanStartGame()
	{
		return IsFull() && !gameStarted;
	}

	void StartGame()
	{
		if (CanStartGame())
			gameStarted = true;
	}

	bool HasStarted()
	{
		return gameStarted;
	}
};