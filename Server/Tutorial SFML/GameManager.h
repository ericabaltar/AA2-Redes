#pragma once
#include <vector>
#include "GameRoom.h"

#define GM GameManager::Instance()

class GameManager
{
public: 
	static GameManager* Instance() {
		static GameManager gm;

		return &gm;
	}

	void AddRoom(GameRoom room) { 
		//TODO: Send Room to GameServer
	}
};

