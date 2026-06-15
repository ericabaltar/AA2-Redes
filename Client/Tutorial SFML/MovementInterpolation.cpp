#include "MovementInterpolation.h" 
#include <algorithm> 
#include <iostream>

//Codigo proporcionado por IA ligeramente modificado

static inline Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
	Vector2 r;
	r.x = a.x * (1.f - t) + b.x * t;
	r.y = a.y * (1.f - t) + b.y * t;
	return r;
}

static inline float Smoothstep(float t) {
	return t * t * (3.0f - 2.0f * t);
}

MovementInterpolation::MovementInterpolation() {
	// Ya no anadimos datos de prueba. Se inicializa correctamente desde el Enter de GameScene.
}

void MovementInterpolation::AddPacket(const User& user, const MovementPacket& packet) {
	if (packetList.find(user) == packetList.end()) {
		packetList[user] = std::vector<MovementPacket>();
		currentMovementPacketIDList[user] = notStartedInterpolatingID;
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

void MovementInterpolation::Update(float dt) {
	ExecuteInterpolation(dt);
}

Vector2 MovementInterpolation::GetInterpolatedPosition(const User& user) const {
	auto it = interpolatedPositions.find(user);
	if (it != interpolatedPositions.end())
		return it->second;
	return Vector2{ 0.f, 0.f };
}

void MovementInterpolation::ExecuteInterpolation(float dt) {
	for (auto& kv : packetList) {
		const User user = kv.first;
		auto& packets = kv.second;

		if (packets.empty())
			continue;

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

				if (idDiff <= 0) {
					idDiff = 1;
				}

				float duration = idDiff * baseTickTime * speedMultiplier;

				if (duration <= 0.f) {
					duration = baseTickTime * speedMultiplier;
				}

				interpolationDurationMap[user] = duration;
				interpolationTimeMap[user] = 0.f;
			}

			float need = interpolationDurationMap[user] - interpolationTimeMap[user];
			float step = std::min(need, remaining);
			interpolationTimeMap[user] += step;
			remaining -= step;

			float t = interpolationDurationMap[user] > 0.f ? interpolationTimeMap[user] / interpolationDurationMap[user] : 1.f;

			if (t > 1.f) {
				t = 1.f;
			}

			float tEff = Smoothstep(t);

			interpolatedPositions[user] = Lerp(interpolationStartMap[user], target.pos, tEff);

			if (interpolationTimeMap[user] >= interpolationDurationMap[user] - interPolationTimeThreshold)
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