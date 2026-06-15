#pragma once
#include "PlayerCharacter.h"
#include <iostream>

#define MM MatchManager::Instance()

class MatchManager
{
public:
	static MatchManager* Instance()
	{
		static MatchManager mm;
		return &mm;
	}

private:
	bool gameStarted = false;
	int roomId = -1;
	int playerIndex = -1;

	PlayerCharacter* localPlayer = nullptr;
	Character* enemyPlayer = nullptr;

public:
	void Initialize(int room, int index)
	{
		roomId = room;
		playerIndex = index;
		gameStarted = false;
	}

	void Reset()
	{
		gameStarted = false;
		roomId = -1;
		playerIndex = -1;

		localPlayer = nullptr;
		enemyPlayer = nullptr;
	}

	void StoreCharacters(PlayerCharacter* player, Character* enemy)
	{
		localPlayer = player;
		enemyPlayer = enemy;
	}

	void StartGame()
	{
		gameStarted = true;

		std::cout << "Iniciando partida. ID: " << roomId << " | PlayerIndex: " << playerIndex << std::endl;
	}

	bool GetGameStarted() const { return gameStarted; }
	int GetRoomId() const { return roomId; }
	int GetPlayerIndex() const { return playerIndex; }

	PlayerCharacter* GetLocalPlayer() const { return localPlayer; }
	Character* GetEnemyPlayer() const { return enemyPlayer; }

	void HandleEnemyQuack()
	{
		enemyPlayer->Quack();
	}

private:
	MatchManager() = default;
	MatchManager(const MatchManager&) = delete;
	MatchManager& operator=(const MatchManager&) = delete;
	~MatchManager() = default;
};