#pragma once
#include <map>
#include <vector>
#include <algorithm>
#include "User.h"
#include "MovementPacket.h"
#include "Vector2.h"

class MovementInterpolation
{
private:
	std::map<User, std::vector<MovementPacket>> packetList;
	std::map<User, int> currentMovementPacketIDList;

	std::map<User, Vector2> interpolatedPositions;

	std::map<User, float> interpolationTimeMap;
	std::map<User, float> interpolationDurationMap;
	std::map<User, Vector2> interpolationStartMap;
	std::map<User, Vector2> interpolationTargetMap;

	const float baseTickTime = 0.05f;          
	float speedMultiplier = 25.0f;
	bool useSmoothstep = true;

public:
	MovementInterpolation();

	void AddPacket(const User& user, const MovementPacket& packet);

	void Update(float dt);

	Vector2 GetInterpolatedPosition(const User& user) const;

	bool HasActiveInterpolation(const User& user) const;
};