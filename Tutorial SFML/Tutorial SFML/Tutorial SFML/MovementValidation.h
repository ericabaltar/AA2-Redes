#pragma once
#include <map>
#include "MovementPacket.h"
#include <vector>
#include "User.h"

std::vector<User> gameUsers = { {"eauna", 0, 0, 0, 1.0f} };

class MovementValidation
{
private:
	std::map<User, std::vector<MovementPacket>> packetList;
	float validationTolerance = 0.5f;

public:
	std::map<User, MovementPacket> ValidatePackets() {
		std::map<User, MovementPacket> validatedPackets;
		for (User user : gameUsers) {
			validatedPackets[user] = ValidateUserPackets(user);
			packetList[user].clear();
			packetList[user].push_back(validatedPackets[user]);
		}

		return validatedPackets;
	}

	void AddPacket(User user, MovementPacket packet) { packetList[user].push_back(packet); }

private:
	MovementPacket ValidateUserPackets(User user) {
		MovementPacket resultPacket = packetList[user][0];
		for (int i = 1; i < packetList[user].size(); ++i) {
			
			if (packetList[user][i].ID < resultPacket.ID) continue;

			int physicsMult = packetList[user][i].ID - resultPacket.ID;

			//Early exit to error
			if (std::abs((packetList[user][i].pos - resultPacket.pos).SqrMagnitude() - physicsMult * physicsMult * user.speed * user.speed) 
				> validationTolerance) 
				return resultPacket;

			resultPacket = packetList[user][i];
		}
	}
};

