#pragma once

#include <SFML/Network.hpp> 
#include <iostream> 
#include <queue> 
#include "ServerPacketTypeManager.h" 
#include "Task.h" 
#include "UdpManager.h"
#include "MovementPacket.h"

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

	sf::Packet receivePacket;

public:
	void Init();
	void Update();

	inline void AddTask(Task task) { pendingTasks.push(task); }

	inline void CloseServer() { closeServer = true; }
	inline bool GetCloseServer() { return closeServer; }

	void SendMovement(const sf::IpAddress& ip, unsigned short port, MovementPacket movement, bool isPlayer);
	void SendShot(const sf::IpAddress& ip, unsigned short port, bool towardsRight);
	void SendTaunt(const sf::IpAddress& ip, unsigned short port);

	inline UdpManager* GetUdpManager() { return &udp; }

private:
	NetworkManager() = default;
	NetworkManager(const NetworkManager& nt) = delete;
	NetworkManager& operator=(const NetworkManager& nt) = delete;
	~NetworkManager() = default;

	void EstablishConnectionWithLauncherServer();
	void HandleReceivedTcpPackets();
};