#pragma once
#include <vector>
#include "User.h"
#include <map>
#include "MovementPacket.h"

#define MovM MovementManager::Instance()
#define MOVEMENT_VALIDATION_TIME_IN_MILLISECONDS 20.0f 

class MovementManager
{
	std::vector<User> gameUsers = { {"eauna", 0, 0, 0, 1.0f} };

private:
	std::map<User, std::vector<MovementPacket>> packetList;
	float validationTolerance = 0.5f;
	sf::Clock clock;

public:
	static MovementManager* Instance() {
		static MovementManager mm;

		return &mm;
	}

	void ValidatePackets() {
		std::map<User, MovementPacket> validatedPackets;
		for (User user : gameUsers) {
			validatedPackets[user] = ValidateUserPackets(user);
			packetList[user].clear();
			packetList[user].push_back(validatedPackets[user]);
		}
	}

	void AddPacket(User user, MovementPacket packet) { packetList[user].push_back(packet); }

	void Update() {
		if (clock.getElapsedTime().asMilliseconds() < MOVEMENT_VALIDATION_TIME_IN_MILLISECONDS) return;

		clock.restart();
		ValidatePackets();
	}

private:
	MovementPacket ValidateUserPackets(User user) {
		MovementPacket resultPacket = packetList[user][0];
		for (int i = 1; i < packetList[user].size(); ++i) {

			if (packetList[user][i].ID < resultPacket.ID) continue;

			int physicsMult = packetList[user][i].ID - resultPacket.ID;

			//Early exit to error
			if (std::abs((packetList[user][i].pos - resultPacket.pos).SqrMagnitude() - physicsMult * physicsMult * user.speed * user.speed)
			> validationTolerance) {
				//TODO: Send notification to reconcile!!!
				return resultPacket;
			}

			resultPacket = packetList[user][i];
		}
	}
};

