#pragma once
#include <SFML/Network.hpp>
#include <optional>
#include <string>

class Player
{
public:
	Player()
		: client(nullptr), name(""), points(0), port(0) {
	}

	Player(sf::TcpSocket* playerClient, std::string playerName, int playerPoints)
		: client(playerClient), name(playerName), points(playerPoints), port(0) {
	}

	sf::TcpSocket* client;
	std::string name;
	int points;

	std::optional<sf::IpAddress> ip;
	unsigned short port;
};