#include "MatchMakingManager.h"
#include "NetworkManager.h"
#include <iostream>
#include "GameRoom.h"
#include "GameManager.h"

void MatchMakingManager::AddConnectedPlayer(Player player, GameMode mode)
{
	if (waitingPlayers[mode].empty()) {
		waitingPlayers[mode].push(player);
		return;
	}

	Player other = waitingPlayers[mode].front();
	waitingPlayers[mode].pop();

	GM->AddRoom(GameRoom(other, player, mode));
}
