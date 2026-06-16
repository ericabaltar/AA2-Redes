#pragma once
#include <vector>
#include <utility>
#include "Player.h"

#define PingM PingManager::Instance()

#define PING_THRESHOLD 0.03f
#define PING_MAX_TIME 0.5f

class PingManager
{
public:
	static PingManager* Instance() {
		static PingManager pm;
		return &pm;
	}

private:
	std::vector<std::pair<Player*, float>> pingTimers;
	std::vector<Player*> playersToPing;
	std::vector<Player*> disconnectedPlayers;

public:
	void Update(float dt) {
		for (std::pair<Player*, float>& pair : pingTimers) {
			pair.second += dt;
			if (pair.second >= PING_MAX_TIME) {
				disconnectedPlayers.push_back(pair.first);
			}
			else if (pair.second >= PING_THRESHOLD) {
				playersToPing.push_back(pair.first);
			}
		}
	}

	std::vector<Player*>& GetPlayersToPing() { return playersToPing; }
	std::vector<Player*>& GetDisconnectedPlayers() { return disconnectedPlayers; }

	void ReceivePing(const sf::IpAddress& ip, unsigned short port) {
		for (std::pair<Player*, float>& pair : pingTimers) 
			if (pair.first->udpIp.value() == ip && pair.first->udpPort == port) {
				pair.second = 0;
				return;
			}
	}

	void AddPlayer(Player* player) { pingTimers.push_back({ player, 0 }); }
	void RemovePlayer(Player* player) {
		for (std::vector<std::pair<Player*, float>>::iterator it = pingTimers.begin(); it != pingTimers.end(); it++) 
			if (player == (*it).first) { pingTimers.erase(it); break; }
		for (std::vector<Player*>::iterator it = playersToPing.begin(); it != playersToPing.end(); it++)
			if (player == *it) { playersToPing.erase(it); break; }
		for (std::vector<Player*>::iterator it = disconnectedPlayers.begin(); it != disconnectedPlayers.end(); it++)
			if (player == *it) { disconnectedPlayers.erase(it); break; }
	}
};

