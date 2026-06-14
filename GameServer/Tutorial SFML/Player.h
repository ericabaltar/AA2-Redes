#pragma once
#include <SFML/Network.hpp>

class Player
{
public:
	Player()
		: udpIp(NULL) {}

	Player(sf::IpAddress udpIp, short udpPort, short index)
		: udpIp(udpIp), udpPort(udpPort), index(index) { }

	std::optional<sf::IpAddress> udpIp;
	short udpPort;
	short index;
};