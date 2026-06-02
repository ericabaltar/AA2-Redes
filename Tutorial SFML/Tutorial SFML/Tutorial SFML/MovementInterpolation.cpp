#include "MovementInterpolation.h"
#include <iostream>
#include <cmath>

static inline Vector2 Lerp(const Vector2& a, const Vector2& b, float t)
{
	Vector2 r;
	r.x = a.x * (1.f - t) + b.x * t;
	r.y = a.y * (1.f - t) + b.y * t;
	return r;
}

static inline float Smoothstep(float t)
{
	return t * t * (3.0f - 2.0f * t);
}

MovementInterpolation::MovementInterpolation()
{
	// Prueba temporal (quita después)
	User testUser;
	testUser.nickname = "testOpponent";
	testUser.userIndex = 999;
	testUser.speed = 1.f;

	Vector2 startPos(100.f, 200.f);
	interpolatedPositions[testUser] = startPos;
	currentMovementPacketIDList[testUser] = -1;

	MovementPacket p1; p1.ID = 1; p1.pos = Vector2(200.f, 200.f);
	MovementPacket p2; p2.ID = 2; p2.pos = Vector2(300.f, 800.f);
	MovementPacket p3; p3.ID = 3; p3.pos = Vector2(400.f, 600.f);
	MovementPacket p4; p4.ID = 4; p4.pos = Vector2(500.f, 300.f);

	auto& list = packetList[testUser];
	list.push_back(p1);
	list.push_back(p2);
	list.push_back(p3);
	list.push_back(p4);
}

void MovementInterpolation::AddPacket(const User& user, const MovementPacket& packet)
{
	if (packetList.find(user) == packetList.end())
	{
		packetList[user] = std::vector<MovementPacket>();
		currentMovementPacketIDList[user] = -1;
		interpolatedPositions[user] = packet.pos;
		interpolationTimeMap[user] = 0.f;
		interpolationDurationMap[user] = 0.f;
		interpolationStartMap[user] = interpolatedPositions[user];
		interpolationTargetMap[user] = packet.pos;
	}

	if (packet.ID <= currentMovementPacketIDList[user])
		return;

	Vector2 start = interpolatedPositions[user];
	Vector2 target = packet.pos;

	interpolationStartMap[user] = start;
	interpolationTargetMap[user] = target;
	interpolationTimeMap[user] = 0.f;

	int idDiff = (currentMovementPacketIDList[user] >= 0) ? (packet.ID - currentMovementPacketIDList[user]) : 1;
	if (idDiff <= 0) idDiff = 1;
	float duration = idDiff * baseTickTime * speedMultiplier;
	if (duration <= 0.00001f) duration = baseTickTime * speedMultiplier;
	interpolationDurationMap[user] = duration;
}

void MovementInterpolation::Update(float dt)
{
	for (auto& kv : packetList)
	{
		const User user = kv.first;
		auto& packets = kv.second;

		if (interpolationTargetMap.find(user) == interpolationTargetMap.end())
		{
			if (!packets.empty())
			{
				interpolationTargetMap[user] = packets.back().pos;
				interpolationStartMap[user] = interpolatedPositions[user];
				interpolationTimeMap[user] = 0.f;
				interpolationDurationMap[user] = baseTickTime * speedMultiplier;
			}
			else
				continue;
		}

		float& time = interpolationTimeMap[user];
		float duration = interpolationDurationMap[user];

		time += dt;

		float t = (duration > 0.f) ? (time / duration) : 1.f;
		if (t > 1.f) t = 1.f;

		float tEff = useSmoothstep ? Smoothstep(t) : t;

		Vector2 start = interpolationStartMap[user];
		Vector2 target = interpolationTargetMap[user];
		interpolatedPositions[user] = Lerp(start, target, tEff);

		if (time >= duration)
		{
			if (!packets.empty())
			{
				if (packets.back().pos.x == target.x && packets.back().pos.y == target.y)
				{
					currentMovementPacketIDList[user] = packets.back().ID;
					packets.pop_back();
				}
			}
			interpolationTimeMap[user] = 0.f;
			interpolationDurationMap[user] = 0.f;
		}
	}
}

Vector2 MovementInterpolation::GetInterpolatedPosition(const User& user) const
{
	auto it = interpolatedPositions.find(user);
	if (it != interpolatedPositions.end())
		return it->second;
	return Vector2{ 0.f, 0.f };
}

bool MovementInterpolation::HasActiveInterpolation(const User& user) const
{
	auto it = packetList.find(user);
	if (it != packetList.end())
		return it->second.size() >= 1;
	return false;
}