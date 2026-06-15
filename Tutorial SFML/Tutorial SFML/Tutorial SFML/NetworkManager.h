#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include "ServerPacketTypesManager.h"
#include "MovementPacket.h"
#include "Utils.h"
#include "UdpManager.h"
#include <list>

#define NT NetworkManager::Instance()
#define SERVER_PORT 55000

const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);

class NetworkManager
{
public:
	static NetworkManager* Instance() {
		static NetworkManager nt;

		return &nt;
	}

private:
	bool disconnectFromServer = false;

	sf::TcpSocket socket;
	bool successfulLogin = false;

	UdpManager udp;

	MovementPacket lastValidatedMovementPacket;
	bool hasValidatedMovementPacket = false;

	sf::Packet receivePacket;

public:
	void Init();
	void Update();

	inline void DisconnectFromServer() { disconnectFromServer = true; }
	inline bool GetDisconnectFromServer() { return disconnectFromServer; }
	inline void SetSuccessfulLogin(bool successful) { successfulLogin = successful; }
	inline bool GetSuccessfulLogin() { return successfulLogin; }

	sf::TcpSocket* GetServerSocket();

	void SendLoginAttemptServerPacket(std::string username, std::string password);
	void SendRegisterAttemptServerPacket(std::string username, std::string password);
	void SendLobbyCreateAttemptPacket(std::string lobbyId);
	void SendLobbyJoinAttemptPacket(GameMode mode);
	void SendRankingPetitionServerPacket(int userId);
	void SendMapPetitionServerPacket();

	void SendMatchConnect(int roomId, uint8_t playerIndex);
	void SendMovementPacket(MovementPacket movementPacket);
	void SendTaunt();
	void SendShot(bool towardsRight);

	void SetLastValidatedMovementPacket(const MovementPacket& packet);
	bool GetLastValidatedMovementPacket(MovementPacket& packet);

private:
	NetworkManager() = default;
	NetworkManager(const NetworkManager& nt) = delete;
	NetworkManager& operator=(const NetworkManager& nt) = delete;
	~NetworkManager() = default;

	void EstablishConnectionWithServer();
	void HandleReceivedTcpPackets();
	void SendServerPacket();
};