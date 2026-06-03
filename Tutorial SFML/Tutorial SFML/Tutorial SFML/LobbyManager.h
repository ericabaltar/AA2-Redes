#pragma once
#include <string>

#define LM LobbyManager::Instance()

class LobbyManager
{
public:
	static LobbyManager* Instance() {
		static LobbyManager lm;

		return &lm;
	}
private:
	bool roomJoined = false;
	bool gameStarted = false;
	int playerCount = 1;
	std::string roomId = "";

public:

	void StartGame()
	{
		gameStarted = true;
	}

	bool GetGameStarted() { return gameStarted; }


private:
	LobbyManager() = default;
	LobbyManager(const LobbyManager& lm) = delete;
	LobbyManager& operator=(const LobbyManager& lm) = delete;
	~LobbyManager() = default;

};
