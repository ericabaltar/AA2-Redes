#pragma once
#include <SFML/Network.hpp>

class Player
{
public:
	Player()
		: client(nullptr), name(""), points(0) {}

	Player(sf::TcpSocket* playerClient, std::string playerName, int playerPoints)
		: client(playerClient), name(playerName), points(playerPoints) { }

	sf::TcpSocket* client;
	std::string name;
	int points;
};