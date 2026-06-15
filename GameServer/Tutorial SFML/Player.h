#pragma once 
#include <SFML/Network.hpp>
#include <optional>
#include "Vector2.h"
#include "AABB.h"

class Player {
public:
	Player() : udpIp(std::nullopt), udpPort(0), index(0), collider(Vector2(0.f, 0.f), Vector2(64.f, 85.33f)) {}

	Player(sf::IpAddress udpIp, unsigned short udpPort, short index)
		: udpIp(udpIp), udpPort(udpPort), index(index), collider(Vector2(0.f, 0.f), Vector2(64.f, 85.33f)) {
	}

	std::optional<sf::IpAddress> udpIp;
	unsigned short udpPort;
	short index;

	Vector2 position;
	AABB collider;
	bool facingRight = true;
	int health = 5;
	int lives = 3;
	int irregularities = 0;
};