#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <queue>
#include "ServerPacketTypeManager.h"
#include "Task.h"

#define NT NetworkManager::Instance()
#define BIND_PORT 55000
#define LAUNCHER_SERVER_PORT 55000

const sf::IpAddress LAUNCHER_SERVER_IP = sf::IpAddress(127, 0, 0, 1);

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

	sf::TcpSocket launcherSocket;
	bool disconnectFromLauncherServer = false;

	std::queue<Task> pendingTasks;
	
public:
	void Init();
	
	void Update();
	//void EstablishConnectionWithClient();
	void ReceiveClientPacket();
	//void CheckForDisconnection();
	
	inline void AddTask(Task task) { pendingTasks.push(task); }

	inline void CloseServer() { closeServer = true; }
	inline bool GetCloseServer() { return closeServer; }
	inline bool CheckIfSocketsAreReadyToReceive() { return selector.wait(); }

private:
	NetworkManager() = default;
	NetworkManager(const NetworkManager& nt) = delete;
	NetworkManager& operator=(const NetworkManager& nt) = delete;
	~NetworkManager() = default;

	void EstablishConnectionWithLauncherServer();
	void HandleReceivedTcpPackets();
};

