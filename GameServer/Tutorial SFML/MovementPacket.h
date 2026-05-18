#pragma once
#include <SFML/Network/Packet.hpp>
#include "Vector2.h"

struct MovementPacket
{
public:
	int ID;
	Vector2 pos;
};

inline sf::Packet& operator <<(sf::Packet& packet, const Vector2& vector)
{
	return packet << vector.x << vector.y;
}

inline sf::Packet& operator >>(sf::Packet& packet, Vector2& vector)
{
	return packet >> vector.x >> vector.y;
}

inline sf::Packet& operator <<(sf::Packet& packet, const MovementPacket& movement)
{
	return packet << movement.ID << movement.pos;
}

inline sf::Packet& operator >>(sf::Packet& packet, MovementPacket& movement)
{
	return packet >> movement.ID >> movement.pos;
}

