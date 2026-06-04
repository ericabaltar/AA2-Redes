#pragma once
#include <vector>
#include <iostream>
#include "GameRoom.h"
#include "ServerPacketTypeManager.h"

#define GM GameManager::Instance()

class GameManager
{
public: 
	static GameManager* Instance() {
		static GameManager gm;

		return &gm;
	}

	void AddRoom(GameRoom room) { 
		std::cout << "Sala creada" << std::endl;

		SPTM->SendInfoToStartGame(room);
		
		//TODO: Send Room to GameServer
	}
};

