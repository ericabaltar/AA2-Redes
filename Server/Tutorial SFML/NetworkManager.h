#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <unordered_map>
#include "ServerPacketTypeManager.h"

#define NT NetworkManager::Instance()
#define LISTENER_PORT 55000

class NetworkManager
{
public:
	static NetworkManager* Instance() {
		static NetworkManager nt;

		return &nt;
	}

private:
	bool closeServer;
	sf::TcpListener listener;
	sf::SocketSelector selector;

	std::vector <sf::TcpSocket*> clients;
	sf::TcpSocket* newClient;

	sf::TcpSocket* gameServer;

	std::unordered_map<sf::TcpSocket*, sf::Packet> clientPackets;

public:
	void Init();
	void Update();
	void EstablishConnectionWithClient();
	void ReceiveClientPacket();

	inline void CloseServer() { closeServer = true; }
	inline bool GetCloseServer() { return closeServer; }
	inline bool CheckIfSocketsAreReadyToReceive() { return selector.wait(); }

	inline void SetGameServerSocket(sf::TcpSocket* socket) { gameServer = socket; }
	inline sf::TcpSocket* GetGameServerSocket() { return gameServer; }

private:
	NetworkManager() = default;
	NetworkManager(const NetworkManager& nt) = delete;
	NetworkManager& operator=(const NetworkManager& nt) = delete;
	~NetworkManager() = default;
};