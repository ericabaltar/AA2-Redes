#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include "ServerPacketTypeManager.h"


#define NT NetworkManager::Instance()
#define BIND_PORT 55000

class NetworkManager
{
public:
	static NetworkManager* Instance() {
		static NetworkManager nt;

		return &nt;
	}

private:
	bool closeServer;
	sf::UdpSocket socket;
	sf::SocketSelector selector;

	std::vector <sf::TcpSocket*> clients;
	sf::TcpSocket* newClient;
	
public:
	void Init();
	
	void Update();
	//void EstablishConnectionWithClient();
	void ReceiveClientPacket();
	//void CheckForDisconnection();
	
	inline void CloseServer() { closeServer = true; }
	inline bool GetCloseServer() { return closeServer; }
	inline bool CheckIfSocketsAreReadyToReceive() { return selector.wait(); }

private:
	NetworkManager() = default;
	NetworkManager(const NetworkManager& nt) = delete;
	NetworkManager& operator=(const NetworkManager& nt) = delete;
	~NetworkManager() = default;
};

