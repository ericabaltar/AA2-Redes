#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <queue>
#include "ServerPacketTypeManager.h"
#include "Task.h"
#include "UdpManager.h"

#define NT NetworkManager::Instance()
#define MAIN_SERVER_PORT 55000

const sf::IpAddress MAIN_SERVER_IP = sf::IpAddress(127, 0, 0, 1);

class NetworkManager
{
public:
	static NetworkManager* Instance() {
		static NetworkManager nt;

		return &nt;
	}

private:
	bool closeServer = false;
	
	UdpManager udp;

	sf::TcpSocket mainServerSocket;

	std::queue<Task> pendingTasks;
	
public:
	void Init();
	
	void Update();
	//void EstablishConnectionWithClient();
	//void CheckForDisconnection();
	
	inline void AddTask(Task task) { pendingTasks.push(task); }

	inline void CloseServer() { closeServer = true; }
	inline bool GetCloseServer() { return closeServer; }

private:
	NetworkManager() = default;
	NetworkManager(const NetworkManager& nt) = delete;
	NetworkManager& operator=(const NetworkManager& nt) = delete;
	~NetworkManager() = default;

	void EstablishConnectionWithLauncherServer();
	void HandleReceivedTcpPackets();
};

