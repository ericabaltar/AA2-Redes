#pragma once
#include <vector>
#include <utility>
#include <map>
#include "Player.h"
#include "MovementPacket.h"

#define MovM MovementManager::Instance()

#define MAX_INFRACTIONS 3

#define MAX_SPEED_X 1500.f
#define MAX_SPEED_Y 2000.f

class MovementManager
{
public:
	static MovementManager* Instance() {
		static MovementManager mm;

		return &mm;
	}

private:
	float validationTolerance = 0.5f;
	std::vector<std::pair<Player*, MovementPacket>> lastValidPackets;
	std::map<Player*, int> infractions;
	std::vector<Player*> cheaters;

public:
	MovementPacket ValidatePacket(Player* player, MovementPacket movement) {
		for (std::pair<Player*, MovementPacket>& lastValid : lastValidPackets) {
			if (lastValid.first == player) {

				int physicsMult = lastValid.second.ID - movement.ID;

				//Not considered an infraction
				if (physicsMult <= 0) 
					return lastValid.second;

				//Early exit to error
				if (std::abs(std::pow(movement.pos.x - lastValid.second.pos.x, 2) - physicsMult * physicsMult * MAX_SPEED_X * MAX_SPEED_X)
				> validationTolerance ||
					std::abs(std::pow(movement.pos.y - lastValid.second.pos.y, 2) - physicsMult * physicsMult * MAX_SPEED_Y * MAX_SPEED_Y)
				> validationTolerance) {
					infractions[player]++;
					if (infractions[player] >= MAX_INFRACTIONS) cheaters.push_back(player);
					return lastValid.second;
				}

				lastValid.second = movement;
				return movement;
			}
		}
	}
};

