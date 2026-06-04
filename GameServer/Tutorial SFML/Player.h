#pragma once
#include <SFML/Network.hpp>

class Player
{
public:
	Player()
		: udpIp(NULL), name(""), points(0) {}

	Player(sf::IpAddress udpIp, short udpPort, std::string playerName, int playerPoints)
		: udpIp(udpIp), udpPort(udpPort), name(playerName), points(playerPoints) { }

	std::optional<sf::IpAddress> udpIp;
	short udpPort;
	std::string name;
	int points;
};