#pragma once
#include <string>
#include <iostream>

#define LM LobbyManager::Instance()

class LobbyManager
{
public:
	static LobbyManager* Instance() {
		static LobbyManager lm;

		return &lm;
	}
private:
	bool gameStarted = false;
	int roomId = -1;
	int playerIndex = -1;

public:

	void StartGame(int room, int index)
	{
		gameStarted = true;
		std::cout << "Iniciando partida. ID: " << room << " | " << "Indice de player: " << index << std::endl;
	}

	bool GetGameStarted() { return gameStarted; }
	int GetRoomId() { return roomId; }
	int GetPlayerIndex() { return playerIndex; }

private:
	LobbyManager() = default;
	LobbyManager(const LobbyManager& lm) = delete;
	LobbyManager& operator=(const LobbyManager& lm) = delete;
	~LobbyManager() = default;

};
