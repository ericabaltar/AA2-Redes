#pragma once
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include "MovementPacket.h"

class MovementPrediction
{
private:
	float sendTimer = 0.f;
	const float sendInterval = 2.f;

	int currentID;

public:
	MovementPrediction()
		: currentID(0) {}

	bool ShouldSendPacket(float dt)
	{
		sendTimer += dt;

		bool shouldSend = false;

		while (sendTimer >= sendInterval)
		{
			sendTimer -= sendInterval;
			shouldSend = true;
		}

		return shouldSend;
	}

	MovementPacket CreateMovementPacket(Vector2 position)
	{
		MovementPacket movementPacket;
		
		movementPacket.ID = currentID;
		movementPacket.pos.x = position.x;
		movementPacket.pos.y = position.y;

		std::cout << "Paquete de movimiento con ID " << currentID << " creado." << std::endl;

		currentID++;

		return movementPacket;
	}
};