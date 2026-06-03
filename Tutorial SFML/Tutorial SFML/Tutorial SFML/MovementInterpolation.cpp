#include "MovementInterpolation.h"
#include <iostream>
#include <cmath>

//Código proporcionado por IA ligeramente modificado

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
	MovementPacket p2; p2.ID = 2; p2.pos = Vector2(300.f, 400.f);
	MovementPacket p3; p3.ID = 3; p3.pos = Vector2(400.f, 100.f);
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

	packetList[user].push_back(packet);
}

void MovementInterpolation::Update(float dt)
{
	ExecuteInterpolation(dt);
}

Vector2 MovementInterpolation::GetInterpolatedPosition(const User& user) const
{
	auto it = interpolatedPositions.find(user);
	if (it != interpolatedPositions.end())
		return it->second;
	return Vector2{ 0.f, 0.f };
}

void MovementInterpolation::ExecuteInterpolation(float dt)
{
	for (auto& kv : packetList)
	{
		const User user = kv.first;
		auto& packets = kv.second;

		if (packets.empty())
			continue;

		// asegurar estado inicial
		if (interpolatedPositions.find(user) == interpolatedPositions.end())
			interpolatedPositions[user] = packets.front().pos;
		if (interpolationTimeMap.find(user) == interpolationTimeMap.end())
			interpolationTimeMap[user] = 0.f;
		if (interpolationDurationMap.find(user) == interpolationDurationMap.end())
			interpolationDurationMap[user] = 0.f;
		if (interpolationStartMap.find(user) == interpolationStartMap.end())
			interpolationStartMap[user] = interpolatedPositions[user];

		float remaining = dt;

		while (remaining > 0.f && !packets.empty())
		{
			MovementPacket target = packets.front();

			if (target.ID <= currentMovementPacketIDList[user])
			{
				packets.erase(packets.begin());
				continue;
			}

			if (interpolationDurationMap[user] <= 0.f)
			{
				interpolationStartMap[user] = interpolatedPositions[user];

				int idDiff = (currentMovementPacketIDList[user] >= 0) ? (target.ID - currentMovementPacketIDList[user]) : 1;
				if (idDiff <= 0) idDiff = 1;
				float duration = idDiff * baseTickTime * speedMultiplier;
				if (duration <= 0.f) duration = baseTickTime * speedMultiplier;

				interpolationDurationMap[user] = duration;
				interpolationTimeMap[user] = 0.f;
			}

			float need = interpolationDurationMap[user] - interpolationTimeMap[user];
			float step = std::min(need, remaining);
			interpolationTimeMap[user] += step;
			remaining -= step;

			float t = interpolationDurationMap[user] > 0.f ? interpolationTimeMap[user] / interpolationDurationMap[user] : 1.f;
			if (t > 1.f) t = 1.f;
			float tEff = t * t * (3.0f - 2.0f * t);

			interpolatedPositions[user] = Lerp(interpolationStartMap[user], target.pos, tEff);

			if (interpolationTimeMap[user] >= interpolationDurationMap[user] - 1e-6f)
			{
				currentMovementPacketIDList[user] = target.ID;

				if (!packets.empty())
					packets.erase(packets.begin());

				interpolationTimeMap[user] = 0.f;
				interpolationDurationMap[user] = 0.f;
				continue;
			}
			else
			{
				break;
			}
		}
	}
}
