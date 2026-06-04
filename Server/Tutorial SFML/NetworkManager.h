#pragma once
#include <SFML/Network.hpp>
#include <iostream>
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
	
public:
	void Init();
	void Update();
	void EstablishConnectionWithClient();
	void ReceiveClientPacket();
	void CheckForDisconnection();
	
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

