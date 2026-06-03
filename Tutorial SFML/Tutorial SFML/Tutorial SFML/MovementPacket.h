#pragma once
#include <SFML/Network/Packet.hpp>
#include "Vector2.h"
#include "User.h"

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

struct OponentMovementPacket
{
public:
	int ID;
	User user;
	Vector2 pos;
};

inline sf::Packet& operator <<(sf::Packet& packet, const OponentMovementPacket& movement)
{
	return packet << movement.ID << movement.user << movement.pos;
}

inline sf::Packet& operator >>(sf::Packet& packet, OponentMovementPacket& movement)
{
	return packet >> movement.ID >> movement.user >> movement.pos;
}

inline sf::Packet& operator <<(sf::Packet& packet, const User& user)
{
	return packet << user.nickname << user.score << user.userIndex << user.position << user.speed;
}

inline sf::Packet& operator >>(sf::Packet& packet, User& user)
{
	return packet >> user.nickname >> user.score >> user.userIndex >> user.position >> user.speed;
}
