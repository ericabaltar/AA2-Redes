#pragma once
#include <SFML/Network.hpp>
#include <optional>
#include <string>

class Player
{
public:
	Player()
		: client(nullptr), name(""), points(0), udpPort(0) {
	}

	Player(sf::TcpSocket* playerClient, std::string playerName, int playerPoints)
		: client(playerClient), name(playerName), points(playerPoints), udpPort(0) {
	}

	sf::TcpSocket* client;
	std::string name;
	int points;

	// Endpoint UDP del jugador. Se actualiza cuando el servidor recibe un paquete UDP suyo.
	std::optional<sf::IpAddress> udpIp;
	unsigned short udpPort;
};